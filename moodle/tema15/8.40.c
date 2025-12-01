#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cnt;
int N;

void SigHndlr(int s) {
    if (cnt < N) {
        printf("напечатано %d строк\n", cnt);
    }
    else {
        printf("напечатаны все %d строк\n", cnt);
    }   
}

int main(int argc, char* argv[]) {
    int f;
    f = open(argv[1],O_WRONLY|O_CREAT|O_TRUNC, 0644);
    N = atoi(argv[2]);
    char *s = argv[3];
    char new_line = '\n';
    signal(SIGINT, SigHndlr);
    printf("PID: %d\n", getpid());
    printf("Для статистики: kill -SIGINT %d\n", getpid());
    for (cnt = 0; cnt < N; cnt ++) {
        write(f, s, strlen(s));
        write(f, &new_line, 1);
        sleep(1);
    }
}
