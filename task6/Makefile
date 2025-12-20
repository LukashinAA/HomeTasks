CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = tiger
SOURCE = tiger.c
RM = rm -f

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

clean:
	$(RM) $(TARGET)

run: $(TARGET)
	./$(TARGET)

debug: $(SOURCE)
	$(CC) $(CFLAGS) -g -o $(TARGET) $(SOURCE)
	gdb ./$(TARGET)

.PHONY: all clean run debug