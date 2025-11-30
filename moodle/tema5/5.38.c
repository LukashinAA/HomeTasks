#include <stdio.h>
void f(int x[], int m, int n) {
    int dop[m];
    int j = 0;
    for (int i = m-n; i < m; i++) {
        dop[j] = x[i];
        j++;
    }
    for (int i = m - 1; i >= n; i--) 
        x[i] = x[i - n];

    for (int i = 0; i < n; i++) 
        x[i] = dop[i];
    
}

int main() {
    int x[] = {1,2,3,4,5,6};
    int m = sizeof(x) / sizeof(x[0]);
    f(x,m,3);
    for (int i = 0; i < m; i++) 
        printf("%d ",x[i]);
}
