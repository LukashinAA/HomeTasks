#include <stdio.h>
double maximum(double x[], int n) {
    double max = 0;
    for (int i = 0; i <= (n-1)/2; i++) {
        if (x[i] + x[n-i-1] > max)
            max =(x[i] + x[n-i-1]); 
    }
    return max;
}

int main() {
    double x[] = {3,8,7,4,10,15,7,8,9,10};
    printf("max summa = %lf\n", maximum(x,10));
}
