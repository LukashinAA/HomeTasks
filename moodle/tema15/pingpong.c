#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_CNT 10
pid_t target_pid;
int cnt;
int fd[2];


void SigHndlr(int s) {
    signal(SIGUSR1,SigHndlr);
    if (cnt < MAX_CNT) {
        read(fd[0],&cnt,sizeof(int));
        printf("%d\n",cnt);
        cnt++;
        write(fd[1],&cnt,sizeof(int));
        kill(target_pid,SIGUSR1);
    }
    else {
        if (target_pid == getppid()) {
            printf("уничтожаем дочерний\n");
            close(fd[0]);
            close(fd[1]);
            exit(0);
        }
        else {
            kill(target_pid, SIGUSR1);
        }
    }
}

int main() {
    signal(SIGUSR1, SigHndlr);
    cnt = 0;
    pipe(fd);
    target_pid = fork();
    if (target_pid > 0) {
           wait(NULL);  
           printf("уничтожаем родителя\n");
           close(fd[0]);
           close(fd[1]);
           exit(0);
    }
    else {
        target_pid = getppid(); 
        write(fd[1], &cnt, sizeof(int));
        kill(target_pid, SIGUSR1);
        while (1) 
            pause();
    }
}
