#include <unistd.h>    // для fork(), dup2(), execlp(), close()
#include <sys/wait.h>  // для wait()
#include <fcntl.h>     // для open() и флагов O_*
#include <stdlib.h>    // для exit()
#include <stdio.h>     // для perror(), printf()
int main(int argc, char *argv[]) {
    printf("Запускаем %d команд параллельно...\n", argc - 1);
    for (int i = 1; i < argc; i++) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("Ошибка fork");
            return 1;
        }

        if (pid == 0) {
            printf("[PID: %d] Запущена: %s\n", getpid(), argv[i]);
            execlp(argv[i], argv[i], NULL);
            perror("   Ошибка: команда не найдена");
            exit(1);
        } else {
            printf("Родитель создал процесс для: %s (PID: %d)\n", argv[i], pid);
        }
    }

    printf("\nОжидаем завершения всех процессов...\n");
    for (int i = 1; i < argc; i++) {
        int status;
        pid_t finished_pid = wait(&status);
        printf("Процесс %d завершился\n", finished_pid);
    }

    printf("Все %d команд выполнены параллельно!\n", argc - 1);
    return 0;
}
