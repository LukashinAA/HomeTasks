#include <stdio.h>      
#include <dirent.h>     
#include <errno.h>      
#include <string.h>     

int main(int argc, char *argv[]) {
    const char *path = ".";  
    
    if (argc > 1) 
        path = argv[1];
    
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("ls");  
        return 1;
    }
    
    struct dirent *entry;
    
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] != '.') 
         printf("%s  ", entry->d_name);  
    }
    printf("\n");  
    
    closedir(dir);  
    return 0;
}
