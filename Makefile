fCC=gcc
CFLAGS=-I.
DEPS=parser.h psinfo.h
OBJ=parser.o psinfo.o prompt.o 

%.o: %.c $(DEPS)
	$(CC) -Wall -c -o $@ $< $(CFLAGS)

# Makefile example
cli: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f cli $(OBJ) *~ 

