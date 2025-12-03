#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// pipe для передачи выбора пользователя от родителя к ребёнку
int choice_pipe[2];

void SigHndlr(int s) {
    char choice;
    printf("Строка превысила N символов\n");
    printf("D - не записывать строку в формируемый файл\n");
    printf("C - записать только первые N символов\n");
    printf("A - прекратить копирование\n");
    scanf(" %c", &choice); // пробел перед %c чтобы пропустить \n
    write(choice_pipe[1], &choice, 1);
}

int main(int argc, char* argv[]) {
    FILE* f1 = fopen(argv[1], "r");
    FILE* f2 = fopen(argv[2], "w");
    int N = atoi(argv[3]);
    
    if (!f1 || !f2) {
        perror("Ошибка открытия файла");
        exit(1);
    }

    signal(SIGUSR1, SigHndlr);

    pipe(choice_pipe);
    int done_pipe[2];
    pipe(done_pipe);
    
    pid_t pid = fork();
    
    if (pid == 0) { 
        close(choice_pipe[1]); 
        close(done_pipe[0]);   

        char *buf = NULL;
        size_t bufsize = 0;
        char choice;
        
        while (getline(&buf, &bufsize, f1) != -1) {
            if (strlen(buf) > N) { 
                kill(getppid(), SIGUSR1); 
                read(choice_pipe[0], &choice, 1); 
                
                if (choice == 'D') {
                    continue; 
                } 
                else if (choice == 'C') {
                    for (int i = 0; i < N; i++) {
                        fputc(buf[i], f2);
                    }
                } 
                else if (choice == 'A') {
                    break; 
                }
            } 
            else {
                fputs(buf, f2); 
            }
        }
        
        free(buf);
        fclose(f1);
        fclose(f2);
        
        char done = '1';
        write(done_pipe[1], &done, 1);
        
        close(choice_pipe[0]);
        close(done_pipe[1]);
        exit(0);
    } 
    else { 
        close(choice_pipe[0]); 
        close(done_pipe[1]);   
        
        char done;
        read(done_pipe[0], &done, 1); 
        
        wait(NULL); 
        printf("Копирование завершено\n");
        
        close(choice_pipe[1]);
        close(done_pipe[0]);
        exit(0);
    }
}
