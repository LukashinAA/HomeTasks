#include <stdio.h>    
#include <stdlib.h>   
#include <ctype.h>    

void process_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("wc");
        return;  
    }
    
    int lines = 0;
    int words = 0;
    int chars = 0;
    int in_word = 0;
    char c;
    
    while ((c = fgetc(file)) != EOF) {
        chars++;  
        if (c == '\n') 
            lines++;  
        if (isspace(c)) 
            in_word = 0;   
        else if (!in_word) {
            in_word = 1;
            words++;     
        }
    }
    
    fclose(file);
    
    printf("%d %d %d %s\n", lines, words, chars, filename);
}

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        process_file(argv[i]);
    }
    
    return 0;
}
