#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {

    int main_pipe[2];
    pipe(main_pipe);
    
    // SUBSHELL 1: ((pr1 | pr2); pr3)
    if (fork() == 0) {
        close(main_pipe[0]);                 // не читаем
        dup2(main_pipe[1], 1);               // stdout → main_pipe
        close(main_pipe[1]);
        
        // ВНУТРЕННИЙ PIPE для pr1 | pr2
        int inner_pipe[2];
        pipe(inner_pipe);
        
        // pr1 (пишет в inner_pipe)
        if (fork() == 0) {
            close(inner_pipe[0]);
            dup2(inner_pipe[1], 1);
            close(inner_pipe[1]);
            execlp(argv[1], argv[1], argv[2], NULL);
            exit(1);
        }
        
        // pr2 (читает из inner_pipe, пишет в stdout subshell'а)
        if (fork() == 0) {
            close(inner_pipe[1]);
            dup2(inner_pipe[0], 0);
            close(inner_pipe[0]);
            execlp(argv[3], argv[3], NULL);
            exit(1);
        }
        
        // Закрываем inner_pipe в subshell
        close(inner_pipe[0]);
        close(inner_pipe[1]);
        
        // Ждём завершения pr1 и pr2
        wait(NULL);
        wait(NULL);
        
        // pr3 (после ;) - пишет в stdout subshell'а
        execlp(argv[4], argv[4], argv[5], NULL);
        exit(1);
    }
    wait(NULL);
    // pr4 (читает из main_pipe)
    close(main_pipe[1]);
    if (fork() == 0) {
        dup2(main_pipe[0], 0);
        close(main_pipe[0]);
        execlp(argv[6], argv[6], NULL);
        exit(1);
    }
    
    // Родитель закрывает и ждёт
    close(main_pipe[0]);
    wait(NULL);
    
    return 0;
}
