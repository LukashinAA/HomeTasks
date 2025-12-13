CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = myshell
OBJS = main.o lexer.o tree.o executor.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c lexer.h tree.h executor.h
	$(CC) $(CFLAGS) -c main.c

lexer.o: lexer.c lexer.h
	$(CC) $(CFLAGS) -c lexer.c

tree.o: tree.c tree.h lexer.h
	$(CC) $(CFLAGS) -c tree.c

executor.o: executor.c executor.h tree.h
	$(CC) $(CFLAGS) -c executor.c

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

debug: CFLAGS += -g
debug: clean $(TARGET)

.PHONY: all clean run debug