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
#include <fcntl.h>

#define PORT 8080

FILE *stats_file;
int server_running = 1;
int sfd;

void handle_sigint(int sig) {
    printf("\nServer shutting down...\n");
    server_running = 0;
    close(sfd);
}

void handle_sigchld(int sig) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

void save_number(int num) {
    FILE *f = fopen("stat.txt", "a");
    if (f) {
        fprintf(f, "%d\n", num);
        fclose(f);
    }
}

void print_statistics() {
    FILE *f = fopen("stat.txt", "r");
    
    int numbers[1000] = {0};
    int counts[1000] = {0};
    int total = 0;
    int num;
    int max_count = 0;
    int most_popular = 0;
    
    while (fscanf(f, "%d", &num) == 1) {
        int found = 0;
        for (int i = 0; i < total; i++) {
            if (numbers[i] == num) {
                counts[i]++;
                found = 1;
                break;
            }
        }
        
        if (!found && total < 1000) {
            numbers[total] = num;
            counts[total] = 1;
            total++;
        }
    }
    fclose(f);
    
    for (int i = 0; i < total; i++) {
        if (counts[i] > max_count) {
            max_count = counts[i];
            most_popular = numbers[i];
        }
    }
    
     if (total > 0) 
        printf("Самое частое число: %d (встретилось %d раз)\n", most_popular, max_count);
            
    remove("stat.txt");
}

void handle_client(int fd) {
    char buf[256];
    int inc = 1;
    int n;
    
    while (1) {
        memset(buf, 0, sizeof(buf));
        n = read(fd, buf, sizeof(buf) - 1);
        if (n <= 0) break;
        
        if (buf[n-1] == '\n') buf[n-1] = '\0';
        
        char *p = buf;
        while (*p == ' ') p++;
        
        if (strcmp(p, "\\-") == 0) {
            write(fd, "Goodbye!\n", 9);
            break;
        }
        else if (p[0] == '+') {
            int val = atoi(p + 1);
            save_number(val);
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
            char *endptr;
            int num = strtol(p, &endptr, 10);
            
            if (*endptr == '\0') {
                save_number(num);
                
                int result = num + inc;
                sprintf(buf, "%d\n", result);
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
    int cfd;
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    
    remove("stat.txt");
    
    signal(SIGCHLD, handle_sigchld);
    signal(SIGINT, handle_sigint);
    
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
    printf("Press Ctrl+C to see statistics and exit\n");
    
    while (server_running) {
        cfd = accept(sfd, (struct sockaddr*)&addr, &len);
        if (cfd < 0) {
            if (!server_running) break;
            perror("accept");
            continue;
        }
        
        if (fork() == 0) {
            signal(SIGINT, SIG_IGN);
            close(sfd);
            handle_client(cfd);
            exit(0);
        } else {
            close(cfd);
        }
    }
    
    close(sfd);
    printf("Ожидание завершения клиентов...\n");
    
    while (waitpid(-1, NULL, 0) > 0);
    
    print_statistics();
    
    return 0;
}
