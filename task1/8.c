#include <stdio.h>

int main() {
    printf("=== Операция %% с отрицательными операндами ===\n\n");
    int x;
    scanf("%d", &x);
    printf("7 %% 3 = %d\n", 7 % 3);
    printf("7 %% -3 = %d\n", 7 % -3);
    printf("-7 %% 3 = %d\n", -7 % 3);
    printf("-7 %% -3 = %d\n", -7 % -3);
    printf("-7 / -3 = %d\n", -7 / -3); 

    
    return 0;
}
