#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <pwd.h>
#include <unistd.h>
#include <time.h>
#include <grp.h>

int flag_detailed = 0;
int flag_recursive = 0;

char get_file_type(mode_t mode) {
    if (S_ISREG(mode))  return '-';
    if (S_ISDIR(mode))  return 'd';
    if (S_ISLNK(mode))  return 'l';
    if (S_ISCHR(mode))  return 'c';
    if (S_ISBLK(mode))  return 'b';
    if (S_ISFIFO(mode)) return 'p';
    if (S_ISSOCK(mode)) return 's';
    return '?';
}

void get_permissions(mode_t mode, char *str) {
    str[0] = get_file_type(mode);
    str[1] = (mode & S_IRUSR) ? 'r' : '-';
    str[2] = (mode & S_IWUSR) ? 'w' : '-';
    str[3] = (mode & S_IXUSR) ? 'x' : '-';
    str[4] = (mode & S_IRGRP) ? 'r' : '-';
    str[5] = (mode & S_IWGRP) ? 'w' : '-';
    str[6] = (mode & S_IXGRP) ? 'x' : '-';
    str[7] = (mode & S_IROTH) ? 'r' : '-';
    str[8] = (mode & S_IWOTH) ? 'w' : '-';
    str[9] = (mode & S_IXOTH) ? 'x' : '-';
    str[10] = '\0';
}

void print_file_info(const char *path, const char *filename) {
    struct stat file_stat;
    char full_path[1024];
    
    if (strcmp(path, ".") == 0) {
        strcpy(full_path, filename);
    } else {
        snprintf(full_path, sizeof(full_path), "%s/%s", path, filename);
    }
    
    if (lstat(full_path, &file_stat) == -1) {
        perror("lstat");
        return;
    }
    
    if (flag_detailed) {
        char permissions[11];
        get_permissions(file_stat.st_mode, permissions);
        printf("%s ", permissions);
        printf("%ld ", file_stat.st_nlink);
        
        struct passwd *pwd = getpwuid(file_stat.st_uid);
        if (pwd != NULL) {
            printf("%s ", pwd->pw_name);
        } else {
            printf("%d ", file_stat.st_uid);
        }
        
        struct group *grp = getgrgid(file_stat.st_gid);
        if (grp != NULL) {
            printf("%s ", grp->gr_name);  
        } else {
            printf("%d ", file_stat.st_gid);
        }      

        printf("%8ld ", file_stat.st_size);
        struct tm *timeinfo = localtime(&file_stat.st_mtime);
        char time_buf[64];
        strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", timeinfo);
        printf("%s ", time_buf);
    }
    
    printf("%s", filename);
    
    if (S_ISLNK(file_stat.st_mode)) {
        char link_target[1024];
        ssize_t len = readlink(full_path, link_target, sizeof(link_target) - 1);
        if (len != -1) {
            link_target[len] = '\0';
            printf(" -> %s", link_target);
        }
    }
    
    printf("\n");
}

void process_directory(const char *path, int is_recursive) {
    DIR *dir = opendir(path);
    if (!dir) {
        perror("ls");
        return;
    }
    
    struct dirent *entry;
    
    if (is_recursive) {
        printf("\n%s:\n", path);
    }
    
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        
        if (entry->d_name[0] == '.') {
            continue;
        }
        
        if (flag_detailed) {
            print_file_info(path, entry->d_name);
        } else {
            printf("%s  ", entry->d_name);
        }
        
        if (flag_recursive) {
            struct stat entry_stat;
            char full_path[1024];
            
            snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
            
            if (lstat(full_path, &entry_stat) == 0 && S_ISDIR(entry_stat.st_mode)) {
                process_directory(full_path, 1);
            }
        }
    }
    
    if (!flag_detailed) {
        printf("\n");
    }
    
    closedir(dir);
}

int main(int argc, char *argv[]) {
    const char *path = ".";
    
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            for (int j = 1; argv[i][j] != '\0'; j++) {
                switch (argv[i][j]) {
                    case 'l': 
                    case 'g': 
                        flag_detailed = 1;
                        break;
                    case 'R': 
                        flag_recursive = 1;
                        break;
                    default:
                        printf("ls: неверный флаг -- '%c'\n", argv[i][j]);
                        return 1;
                }
            }
        } else {
            path = argv[i];
        }
    }
    
    process_directory(path, 0);
    
    return 0;
}
