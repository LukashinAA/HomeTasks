#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#define PORT 8080

void handle_sigchld(int sig) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

void handle_client(int fd) {
    char buf[256];
    int inc = 1;
    int n;
    
    while (1) {
        memset(buf, 0, sizeof(buf));
        n = read(fd, buf, sizeof(buf) - 1);
        if (n <= 0) break;
        
        // убираем перевод строки в конце
        if (buf[n-1] == '\n') buf[n-1] = '\0';
        
        // убираем пробелы в начале
        char *p = buf;
        while (*p == ' ') p++;
        
        if (strcmp(p, "\\-") == 0) {
            write(fd, "Goodbye!\n", 9);
            break;
        }
        else if (p[0] == '+') {
            int val = atoi(p + 1);
            if (val > 0) {
                inc = val;
                write(fd, "OK\n", 3);
            } else {
                write(fd, "Invalid\n", 8);
            }
        }
        else if (strcmp(p, "\\?") == 0) {
            char tmp[32];
            sprintf(tmp, "%d\n", inc);
            write(fd, tmp, strlen(tmp));
        }
        else {
            // пробуем преобразовать в число
            char *endptr;
            int num = strtol(p, &endptr, 10);
            if (*endptr == '\0') {
                sprintf(buf, "%d\n", num + inc);
                write(fd, buf, strlen(buf));
            } else {
                write(fd, "Unknown\n", 8);
            }
        }
    }
    
    close(fd);
    exit(0);
}

int main() {
    int sfd, cfd;
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    
    signal(SIGCHLD, handle_sigchld);
    
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd < 0) {
        perror("socket");
        return 1;
    }
    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);
    
    if (bind(sfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return 1;
    }
    
    if (listen(sfd, 5) < 0) {
        perror("listen");
        return 1;
    }
    
    printf("Server on port %d\n", PORT);
    
    while (1) {
        cfd = accept(sfd, (struct sockaddr*)&addr, &len);
        if (cfd < 0) {
            perror("accept");
            continue;
        }
        
        if (fork() == 0) {
            close(sfd);
            handle_client(cfd);
        } else {
            close(cfd);
        }
    }
    
    return 0;
}