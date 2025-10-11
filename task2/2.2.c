#include <stdio.h>
int main() {
    double x, a, rez, proiz;
    scanf("%lf", &x);
    rez = 0;
    proiz = 0;
    while (scanf("%lf", &a) == 1) {
        proiz = rez + proiz * x;
        rez = x * rez + a;
    }
    printf("%lf\n", rez);
    printf("%lf\n", proiz);
}
