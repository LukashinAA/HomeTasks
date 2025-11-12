#include <stdio.h>

int main(int argc, char *argv[]) {
    int n = 10;
    char *filename = NULL;
    int mode = 0;
    
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            n = 0;
            for (int j = 1; argv[i][j] != '\0'; j++) {
                n = n * 10 + (argv[i][j] - '0');
            }
            mode = 0;
        }
        else if (argv[i][0] == '+') {
            n = 0;
            for (int j = 1; argv[i][j] != '\0'; j++) {
                n = n * 10 + (argv[i][j] - '0');
            }
            mode = 1;
        }
        else {
            filename = argv[i];
        }
    }
    
    if (filename == NULL) {
        printf("Использование: %s [-число] [+число] файл\n", argv[0]);
        return 1;
    }
    
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("tail");
        return 1;
    }
    
    if (mode == 0) {
        int total_lines = 0;
        char ch;
        while ((ch = fgetc(file)) != EOF) {
            if (ch == '\n') total_lines++;
        }
        
        fseek(file, 0, SEEK_SET);
        
        int skip_lines = total_lines - n + 1;
        if (skip_lines < 0) 
            skip_lines = 0;
        for (int i = 0; i < skip_lines; i++) {
            while (fgetc(file) != '\n' && !feof(file));
        }
        
        char line[256];
        while (fgets(line, sizeof(line), file)) {
            printf("%s", line);
        }
    }
    else {
        for (int i = 0; i < n - 1; i++) {
            while (fgetc(file) != '\n' && !feof(file));
        }
        
        char line[256];
        while (fgets(line, sizeof(line), file)) {
            printf("%s", line);
        }
    }
    
    fclose(file);
    return 0;
}
