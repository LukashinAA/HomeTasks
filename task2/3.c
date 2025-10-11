#include <stdio.h>

long iterative(int n) {
    long f0 = 0, f1 = 1;
    if (n == 0) return 0;
    if (n == 1) return 1;
    for (int i = 2; i <= n; i++) {
        f1 = f1 + f0;
        f0 = f1 - f0;
    }
    return f1;
}

long recursia(int n) {
   if (n == 0) return 0;
   if (n == 1) return 1;
   return recursia(n-2) + recursia(n-1);
}

int main() {
    int i, n;
    while (scanf("%d", &n) == 1) {
        printf("%ld\n", iterative(n));
        printf("%ld\n", recursia(n));
    }
}
