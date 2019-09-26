CC=gcc
CFLAGS=-I -Wall.
DEPS = MaquinaDeTuring.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

Sim: SimulacaoMT.o MaquinaDeTuring.o 
	gcc -o Sim SimulacaoMT.o MaquinaDeTuring.o -I.
