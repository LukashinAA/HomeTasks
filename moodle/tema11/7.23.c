#include <stdio.h>

int main(int argc, char* argv[]) {
    FILE *res;
    res = fopen(argv[argc-1], "w");
    for (int i = 1; i < argc-1; i ++) {
        FILE *f;
        f = fopen(argv[i], "r");
        char *line = NULL;
        int n = 0;
        while (getline(&line,&n,f) != -1) 
            fputs(line,res);
        fputc('\n',res);
        fclose(f);
    }
    fclose(res);
}
