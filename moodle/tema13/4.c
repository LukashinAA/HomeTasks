#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    int status;
    pid_t pid1,pid2;
    if (fork() > 0) {
        pid1 = wait(&status);
        printf("1\n");
    }
    else {
        if (fork() > 0) {
            pid2 = wait(&status);
            printf("2 ");
        }
        else 
            printf("3 ");
    }
}
