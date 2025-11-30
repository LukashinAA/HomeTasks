#include <stdio.h>
char f(char s[], int n, int* x) {
    int alf[26] = {0};
    int max = 0;
    char rez;
    for (int i = 0; i <= n-1; i++) {
        alf[s[i]-'a'] += 1;
    }
    for (int i = 0; i <= 25; i++) {
        if (alf[i] > max) {
            max = alf[i];
            rez = 'a' + i;
        }
        else if (alf[i] == max) {
            if (s[i] < rez)
                rez = s[i];
        }
    }
    *x = max;
    return rez;
}

int main() {
    char str[] = "dnbbbbvrisnvajdlllscvaaa";
    int x;
    printf("symvol = %c\n", f(str, sizeof(str), &x));
    printf("count = %d\n", x);
}
