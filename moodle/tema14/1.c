#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    int fd[2], out, in;
    pipe(fd);
    out = fd[1];
    in = fd[0];
    if (fork() == 0) {
        close(in);
        dup2(out,1);
        close(out);
        execlp(argv[1],argv[1], argv[2],NULL);
        exit(-1);
    }   
    close(out);
    if (fork() == 0) {
        dup2(in,0);
        close(in);
        execlp(argv[3],argv[3],NULL);
        exit(-1);
    }
    close(in);
    wait(NULL);
    wait(NULL);
}
