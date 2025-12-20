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
int *tiger_last;
int *keeper_last;
pid_t tiger_pid, keeper_pid;

void kill_children() {
    if (tiger_pid > 0) kill(tiger_pid, SIGTERM);
    if (keeper_pid > 0) kill(keeper_pid, SIGTERM);
    sleep(1);
}

void cleanup() {
    kill_children();
    if (bowl != (int*)-1 && bowl != NULL) shmdt(bowl);
    shmctl(shm_id, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID);
}

void sigint_handler(int sig) {
    exit(0);
}

void sigterm_handler(int sig) {
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
            int eat;
            
            if (*tiger_last == 0) {
                eat = rand() % 3 + 1;
            } else {
                eat = *tiger_last + (rand() % 3 + 1);
            }
            
            if (eat > *bowl) eat = *bowl;
            
            *bowl -= eat;
            *tiger_last = eat;
            
            printf("Тигр съел %d кг. Осталось: %d кг\n", eat, *bowl);
        } else {
            printf("Миска пуста\n");
        }
        
        sem_signal();
    }
}

void keeper() {
    signal(SIGINT, SIG_IGN);
    
    while(1) {
        sleep(rand() % 2 + 1);
        
        sem_wait();
        
        if (*bowl == 0) {
            int add;
            
            if (*keeper_last == 0) {
                add = 10;
            } else {
                add = *keeper_last - 1;
            }
            
            if (add < 1) add = 1;
            
            if (add < *tiger_last) {
                printf("Кормление бессмысленно! Тигр ест %d кг, а можно добавить только %d кг\n", 
                       *tiger_last, add);
                sem_signal();
                kill(getppid(), SIGTERM);
                exit(0);
            }
            
            *bowl = add;
            *keeper_last = add;
            
            printf("Добавлено %d кг\n", add);
        } else {
            printf("В миске %d кг\n", *bowl);
        }
        
        sem_signal();
    }
}

int main() {
    bowl = NULL;
    tiger_last = NULL;
    keeper_last = NULL;
    tiger_pid = 0;
    keeper_pid = 0;
    
    signal(SIGINT, sigint_handler);
    signal(SIGTERM, sigterm_handler);
    atexit(cleanup);
    srand(time(NULL));
    
    shm_id = shmget(IPC_PRIVATE, 3 * sizeof(int), 0666 | IPC_CREAT);
    bowl = (int*)shmat(shm_id, NULL, 0);
    
    tiger_last = bowl + 1;
    keeper_last = bowl + 2;
    
    *bowl = 10;
    *tiger_last = 0;
    *keeper_last = 0;
    
    sem_id = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
    semctl(sem_id, 0, SETVAL, 1);
    
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
    
    while(1) {
        pause();
    }
    
    return 0;
}