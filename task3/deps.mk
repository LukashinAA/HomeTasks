listofwords.o: listofwords.c listofwords.h
scanner.o: scanner.c scanner.h listofwords.h
main.o: main.c scanner.h listofwords.h
