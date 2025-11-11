CC = gcc
CFLAGS = -Wall -Wextra -std=c99
OBJS = main.o List.o
TARGET = task3

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c List.h
	$(CC) $(CFLAGS) -c main.c

List.o: List.c List.h
	$(CC) $(CFLAGS) -c List.c

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean
