#include <unistd.h>    // для fork(), dup2(), execlp(), close()
#include <sys/wait.h>  // для wait()
#include <fcntl.h>     // для open() и флагов O_*
#include <stdlib.h>    // для exit()
#include <stdio.h>     // для perror(), printf()
int main(int argc, char *argv[]) {    
    printf("Запускаем %d команд последовательно...\n", argc - 1);
    
    for (int i = 1; i < argc; i++) {
        printf("\n▷ Команда %d: %s\n", i, argv[i]);
        
        pid_t pid = fork();
        
        if (pid == -1) {
            perror("Ошибка fork");
            return 1;
        }
        
        if (pid == 0) {
            printf("[PID: %d] Выполняем: %s\n", getpid(), argv[i]);
            execlp(argv[i], argv[i], NULL);
            perror("   Ошибка: команда не найдена");
            exit(1);
        } else {
            wait(NULL);
            printf("Команда '%s' завершена\n", argv[i]);
        }
    }
    
    printf("Все %d команд выполнены последовательно!\n", argc - 1);
    return 0;
}
