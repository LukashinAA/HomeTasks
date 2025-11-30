#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    FILE *file;
    int c;
    int count[256] = {0};
    file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Ошибка: не могу открыть файл '%s'\n", argv[1]);
        return 1;
    }
    while ((c = getc(file)) != EOF) {
        count[c] ++;
    }
    fclose(file);
    int maxc = 0;
    int sym = 0;
    for (int i = 0; i < 256; i++) {
        if (count[i] > maxc) {
            maxc = count[i];
            sym = i;
        }
    }
    printf("symvol %c: %d count\n", sym, maxc);
}
