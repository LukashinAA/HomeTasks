#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    int pipe1[2],pipe2[2], out1, in1, out2, in2;
    pipe(pipe1);
    pipe(pipe2);
    out1 = pipe1[1];
    in1 = pipe1[0];
    out2 = pipe2[1];
    in2 = pipe2[0];
    if (fork() == 0) {
        close(in1);
        close(in2);
        close(out2);
        dup2(out1,1);
        close(out1);
        execlp(argv[1],argv[1],NULL);
        exit(-1);
    }   
    if (fork() == 0) {
        dup2(in1,0);
        dup2(out2,1);
        close(in1);
        close(in2);
        close(out1);
        close(out2);
        execlp(argv[2],argv[2],NULL);
        exit(-1);
    }
    close(out1);
    close(out2);
    close(in1);
    if (fork() == 0) {
        dup2(in2,0);
        close(in2);
        execlp(argv[3],argv[3],NULL);
        exit(-1);
    }
    close(in2);
    wait(NULL);
    wait(NULL);
    wait(NULL);
}

