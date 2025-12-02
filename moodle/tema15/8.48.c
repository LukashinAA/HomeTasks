#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

volatile int flag;
FILE* f;
int N;

void SigHndlr(int s) {
    char line[100];
    for (int i = 0; i < N; i ++) {
        if (fgets(line, sizeof(line), f)) {
            printf("%s", line);
        }
        else {
            flag = -1;
            printf("file is over\n");
            break;
        }
    }
}

int main(int argc, char* argv[]) {
    f = fopen(argv[1], "r");
    flag = 0;
    N = atoi(argv[2]);
    signal(SIGINT, SigHndlr);
    while (flag == 0) 
        pause();
    fclose(f);
}
