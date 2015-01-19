all: shopper trace clean

shopper: shopper.o
	clang -g -Wall -w shopper.c -o shopper

trace: trace.o lc4.o parser.o
	clang -g -Wall -w trace.o lc4.o parser.o -o trace

trace.o: trace.c lc4.h parser.h
	clang -g -Wall -c -w trace.c

lc4.o: lc4.c lc4.h parser.h
	clang -g -Wall -c -w lc4.c

parser.o: parser.c lc4.h
	clang -g -Wall -c -w parser.c 

clean:
	rm -rf *o
