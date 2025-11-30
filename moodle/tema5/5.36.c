#include <stdio.h>
#include <string.h>
void f(char s[], int n, char rez[]) {
    int j = 0;
    for (int i = 0; i < n; i+=2) {
        rez[j] = s[i];
        j++;
    }
    for (int i =  1; i < n; i+=2) {
        rez[j] = s[i];
        j++;
    }
}

int main() {
    char s[] = "abcdefg";
    char rez[strlen(s)];
    f(s, strlen(s), rez);  
    printf("resultat = %s\n", rez);
}
