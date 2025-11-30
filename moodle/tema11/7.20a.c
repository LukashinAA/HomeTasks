#include <stdio.h>

int main(int argc, char* argv[]) {
    FILE *f;
    int number;
    int max = 0;
    f = fopen(argv[1], "r");
    while (fscanf(f, "%d", &number) > 0) {
        if (number > max) 
            max = number;
    }   
    printf("%d\n", number);
}
