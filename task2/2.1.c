#include <stdio.h>
int main() {
    double x, a, res;
    scanf("%lf", &x);
    res = 0;
    while (scanf("%lf", &a) == 1) {
        res = x * res + a;
    }
    printf("%lf\n", res);
}
