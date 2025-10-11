#include <stdio.h>

int main() {
    double x1,x2,x, e;
    scanf("%lf", &e);
    while (scanf("%lf", &x) == 1) {
        x1 = 1;
        x2 = (x1 + x/x1)/2;
        while (x1 - x2 >= e || x2 - x1 >= e) {
            x1 = x2;
            x2 = (x1 + x/x1)/2;
        }
        x = x2;
        printf("%.10g\n", x);
    }
}
