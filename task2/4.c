#include <stdio.h>

double str2double(char str[]) {
    double integer_part = 0.0;
    double fractional_part = 0.0;
    double fraction_weight = 0.1;
    int exponent = 0;
    int exponent_sign = 1;
    int has_float_suffix = 0;
    int after_decimal = 0;
    int in_exponent = 0;
    int i = 0;

    while (str[i] != '\0') {
        if (str[i] == 'f' || str[i] == 'F') {
            has_float_suffix = 1;
            i++;
            continue;
        }
                
        if (str[i] == '.') {
            after_decimal = 1;
            i++;
            continue;
        }
        
        if (str[i] == 'e' || str[i] == 'E') {
            in_exponent = 1;
            i++;
        
            if (str[i] == '-') {
                exponent_sign = -1;
                i++;
            } else if (str[i] == '+') 
                i++;
            continue;
        }
        
        if (str[i] >= '0' && str[i] <= '9') {
            int digit = str[i] - '0';
            
            if (in_exponent) {
                exponent = exponent * 10 + digit;
            } else if (after_decimal) {
                fractional_part += fraction_weight * digit;
                fraction_weight *= 0.1;
            } else {
                integer_part = integer_part * 10 + digit;
            }
        }
        
        i++;
    }

    double result = integer_part + fractional_part;
    
    if (exponent > 0) {
        double multiplier = 1.0;
        for (int j = 0; j < exponent; j++) {
            if (exponent_sign == 1) 
                multiplier *= 10.0;
            else multiplier *= 0.1;
        }
        result *= multiplier;
    }
    
        return result;
}

int main() {
    char str[100];
    
    while (scanf("%s", str) == 1) {
        printf("%.10g\n", str2double(str));
    }
    
    return 0;
}
