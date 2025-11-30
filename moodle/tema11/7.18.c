#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    FILE* file;
    char line[100];
    char mline[100] = "";
    int maxlen = 0;
    file = fopen(argv[1],"r");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return 1;
    }
    while (fgets(line, sizeof(line), file) != NULL) {
        if (strlen(line) > maxlen) {
            strcpy(mline, line);
            maxlen = strlen(line);
        }
    }
    printf("max len is: %s\n", mline);
}
