#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>  

int shm_id;
int sem_id;
int *bowl;
pid_t tiger_pid, keeper_pid;

#define MAX_MEAT 10

void kill_children() {
    if (tiger_pid > 0) kill(tiger_pid, SIGTERM);
    if (keeper_pid > 0) kill(keeper_pid, SIGTERM);
    sleep(1);
}

void cleanup() {
    kill_children();
    shmdt(bowl);
    shmctl(shm_id, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID);
}

void sigint_handler(int sig) {
    exit(0);
}

void sem_wait() {
    struct sembuf op = {0, -1, 0};
    semop(sem_id, &op, 1);
}

void sem_signal() {
    struct sembuf op = {0, 1, 0};
    semop(sem_id, &op, 1);
}

void tiger() {
    signal(SIGINT, SIG_IGN);
    
    while(1) {
        sleep(rand() % 3 + 1);
        
        sem_wait();
        
        if (*bowl > 0) {
            int eat = rand() % 3 + 1;
            if (eat > *bowl) eat = *bowl;
            
            *bowl -= eat;
            printf("🐯 Тигр съел %d кг. Осталось: %d кг\n", eat, *bowl);
        } else {
            printf("🐯 Миска пуста\n");
        }
        
        sem_signal();
    }
}

void keeper() {
    signal(SIGINT, SIG_IGN);
    
    while(1) {
        sleep(2);
        
        sem_wait();
        
        if (*bowl == 0) {
            *bowl = MAX_MEAT;
            printf("👨‍🍳 Смотритель наполнил миску: %d кг\n", *bowl);
        } else {
            printf("👨‍🍳 Смотритель проверил: %d кг в миске\n", *bowl);
        }
        
        sem_signal();
    }
}

int main() {
    signal(SIGINT, sigint_handler);
    atexit(cleanup);
    srand(time(NULL));
    
    shm_id = shmget(IPC_PRIVATE, sizeof(int), 0666 | IPC_CREAT);
    bowl = shmat(shm_id, NULL, 0);
    *bowl = MAX_MEAT;
    printf("Миска создана. Начало: %d кг\n", *bowl);
    
    sem_id = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
    semctl(sem_id, 0, SETVAL, 1);
    printf("Семафор создан\n");
    
    tiger_pid = fork();
    if (tiger_pid == 0) {
        tiger();
        exit(0);
    }
    
    keeper_pid = fork();
    if (keeper_pid == 0) {
        keeper();
        exit(0);
    }
    
    printf("Для выхода: Ctrl+C\n");
    
    wait(NULL);
    wait(NULL);
    
    return 0;
}