#include <unistd.h>    // для fork(), dup2(), execlp(), close()
#include <sys/wait.h>  // для wait()
#include <fcntl.h>     // для open() и флагов O_*
#include <stdlib.h>    // для exit()
#include <stdio.h>     // для perror(), printf()
int main(int argc, char *argv[]) {
    
    int file_fd = open(argv[2], O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (file_fd == -1) {
        perror("Ошибка открытия файла");
        return 1;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("Ошибка fork");
        close(file_fd);
        return 1;
    }

    if (pid == 0) {
        dup2(file_fd, STDOUT_FILENO);  
        close(file_fd);
        execlp(argv[1], argv[1], NULL);
        perror("Ошибка execlp");
        exit(1);
    } else {
        close(file_fd);
        wait(NULL);
        printf("Команда '%s' выполнена, вывод добавлен в '%s'\n", argv[1], argv[2]);
    }

    return 0;
}
