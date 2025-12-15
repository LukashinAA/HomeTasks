#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int needprint;

void signal_handler(int sig) {
    needprint = 1;
}

int main(int argc, char *argv[]) {
    size_t buf_size = 64;
    char *s = malloc(buf_size);    
    int c;
    size_t pos = 0;
    FILE* f;
    int maxlen = 0;
    char* maxstr = NULL;
    signal(SIGINT, signal_handler);
    f = fopen(argv[1], "r");
    while ((c = fgetc(f)) != EOF) {
        if (pos >= buf_size - 1) {
            buf_size *= 2;
            s = realloc(s, buf_size);
        }
        if (c == '\n') {
            s[pos] = '\0';
            if (pos-1 > maxlen) {
               maxlen = pos-1;
               maxstr = strdup(s);
            }
            pos = 0;
            sleep(1);
        }
        else {
            s[pos] = c;
            pos ++;
        }
        if (needprint) {
            needprint = 0;
            printf("%s\n",maxstr);
        }
    }
    s[pos] = '\0';
    if (pos-1 > maxlen) {
        maxlen = pos-1;
        maxstr = strdup(s);
    }
    fclose(f);
    free(s);
    printf("%s\n",maxstr);
    free(maxstr);
}

