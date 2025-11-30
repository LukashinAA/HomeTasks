#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char *s;
    int count;
} InfStr;

#define ml 100

int main(int argc, char* argv[]) {
    FILE *f;
    f = fopen(argv[1], "r");    
    InfStr *string = malloc(ml * sizeof(InfStr));
    char line[256];
    int ukaz = 0;
    while (fgets(line, sizeof(line), f) > 0) {
        line[strcspn(line, "\n")] = '\0';
        int found = 0;
        for (int i = 0; i < ukaz; i ++){
            if (strcmp(string[i].s, line) == 0) {
                found = 1;
                string[i].count ++;
                break;
            }
        }

        if (found == 0) {
            string[ukaz].s = malloc(sizeof(line)+1);
            strcpy(string[ukaz].s, line);
            string[ukaz].count = 1;
            ukaz ++;
        }
    }
    fclose(f);

    int max = 0;
    char *maxl = NULL;
    for (int i = 0; i < ukaz; i ++) {
        if (string[i].count > max) {
            max = string[i].count;
            maxl = string[i].s;
        }
    }
    printf("%s\n", maxl);
    printf("%d\n", max);
    free(string);
}
