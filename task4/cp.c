#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char *argv[]) {
    
    if (strcmp(argv[1], argv[2]) == 0) {
        printf("cp: '%s' и '%s' - один и тот же файл\n", argv[1], argv[2]);
        return 1;
    }
    
    int src_fd = open(argv[1], O_RDONLY);
    if (src_fd == -1) {
        perror("cp: исходный файл");
        return 1;
    }
    
    struct stat st;
    fstat(src_fd, &st);
    
    int dst_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, st.st_mode);
    if (dst_fd == -1) {
        perror("cp: целевой файл");
        close(src_fd);
        return 1;
    }
    
    char buffer[4096];
    ssize_t bytes;
    
    while ((bytes = read(src_fd, buffer, sizeof(buffer))) > 0) {
        write(dst_fd, buffer, bytes);
    }
    
    close(src_fd);
    close(dst_fd);
    return 0;
}