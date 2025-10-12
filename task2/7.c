/* <выражение> ::= <слагаемое> { (+|-) <слагаемое> }

<слагаемое> ::= <множитель> { (*|/) <множитель> }

<множитель> ::= <степень> { ^ <степень> }

<степень> ::= цифра | ( <выражение> ) */

#include <stdio.h>
#include <setjmp.h>
#include <ctype.h>

void getlex(void);
int expr(void);
int term(void);
int factor(void);
int power(void);
void error(void);

jmp_buf begin;
char curlex;

int main() {
    int result;
    setjmp(begin);
    printf("==>");
    getlex();
    result = expr();
    if (curlex != '\n') 
        error();
    printf("\n%d\n", result);
    return 0;
}

void getlex() {
    while ((curlex = getchar()) == ' ');
}

void error(void) {
    printf("\nОШИБКА!\n");
    while(getchar() != '\n');
    longjmp(begin, 1);
}

// <выражение> ::= <слагаемое> { (+|-) <слагаемое> }
int expr(void) {
    int e = term();
    while (curlex == '+' || curlex == '-') {
        char op = curlex;
        getlex();
        int t = term();
        if (op == '+') 
            e += t;
        else 
            e -= t;
    }
    return e;
}

// <слагаемое> ::= <множитель> { (*|/) <множитель> }
int term(void) {
    int a = factor();
    while (curlex == '*' || curlex == '/') {
        char op = curlex;
        getlex();
        int f = factor();
        if (op == '*') 
            a *= f;
        else {
            if (f == 0) 
                error();
            a /= f;
        }
    }
    return a;
}

// <множитель> ::= <степень> { ^ <степень> }
int factor(void) {
    int p = power();
    while (curlex == '^') {
        getlex();
        int exp = power();
        if (exp < 0) 
            error();
        int result = 1;
        for (int i = 0; i < exp; i++) {
            result *= p;
        }
        p = result;
    }
    return p;
}

// <степень> ::= цифра | ( <выражение> )
int power(void) {
    int m;
    switch(curlex) {
        case '0': case '1': case '2': case '3': case '4': case '5':
        case '6': case '7': case '8': case '9': 
            m = curlex - '0'; 
            break;
        case '(': 
            getlex(); 
            m = expr();
            if (curlex != ')') 
                error();
            break;
        default: 
            error();
    }
    getlex();
    return m;
}
