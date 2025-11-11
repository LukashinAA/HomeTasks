SRCMODULES = List.c
OBJMODULES = $(SRCMODULES:.c=.o)
CFLAGS = -g -Wall -ansi -pedantic

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

task3: main.o $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@

main.o: main.c List.h
	$(CC) $(CFLAGS) -c main.c -o main.o

ifneq ($(MAKECMDGOALS),clean)
-include deps.mk
endif

deps.mk: main.c $(SRCMODULES)
	$(CC) -MM $^ > $@

clean:
	rm -f *.o task3 deps.mk