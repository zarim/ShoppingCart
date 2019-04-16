CC=gcc
CFLAGS=-I. -pthread -std=c99
DEPS = shopping-cart.h
OBJ = shopping-cart.o main.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)