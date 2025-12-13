CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

all: myshell

myshell: lexer.o tree.o executor.o main.o
	$(CC) $(CFLAGS) -o $@ $^

lexer.o: lexer.c lexer.h
	$(CC) $(CFLAGS) -c lexer.c

tree.o: tree.c tree.h lexer.h
	$(CC) $(CFLAGS) -c tree.c

executor.o: executor.c executor.h tree.h
	$(CC) $(CFLAGS) -c executor.c

main.o: main.c lexer.h tree.h executor.h
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f *.o myshell

.PHONY: all clean