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

#define MAX_MEAT 10

void cleanup() {
    printf("Очистка ресурсов\n");
    shmdt(bowl);
    shmctl(shm_id, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID);
    printf("Ресурсы удалены\n");
}

void sigint_handler(int sig) {
    printf("\nЗавершение\n");
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
    printf("Тигр запущен (PID: %d)\n", getpid());
    
    while(1) {
        sleep(rand() % 3 + 1);
        
        sem_wait();
        
        if (*bowl > 0) {
            int eat = rand() % 3 + 1;
            if (eat > *bowl) eat = *bowl;
            
            *bowl -= eat;
            printf(" ТИГР ||| Съел %d кг. Осталось: %d кг\n", eat, *bowl);
        } else {
            printf(" ТИГР ||| Миска пуста\n");
        }
        
        sem_signal();
    }
}

void keeper() {
    printf("Смотритель запущен (PID: %d)\n", getpid());
    
    while(1) {
        sleep(2);
        
        sem_wait();
        
        if (*bowl == 0) {
            *bowl = MAX_MEAT;
            printf(" СМОТРИТЕЛЬ ||| Наполнил миску: %d кг\n", *bowl);
        } else {
            printf(" СМОТРИТЕЛЬ ||| Проверил: %d кг в миске\n", *bowl);
        }
        
        sem_signal();
    }
}

int main() {
    signal(SIGINT, sigint_handler);
    atexit(cleanup);
    srand(time(NULL));
    
    shm_id = shmget(IPC_PRIVATE, sizeof(int), 0666 | IPC_CREAT);
    bowl = (int*)shmat(shm_id, NULL, 0);
    *bowl = MAX_MEAT;
    printf("Миска создана. Начало: %d кг\n", *bowl);
    
    sem_id = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
    semctl(sem_id, 0, SETVAL, 1);
    printf("Семафор создан\n");
    
    if (fork() == 0) {
        tiger();
        return 0;
    }
    
    if (fork() == 0) {
        keeper();
        return 0;
    }
    
    printf("Для выхода: Ctrl+C\n");
    
    wait(NULL);
    wait(NULL);
    
    return 0;
}