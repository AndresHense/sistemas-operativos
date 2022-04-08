CC=gcc
CFLAGS=-Wall -g
LDFLAGS = -lm

anillo_alu: anillo_alu.c hijos.o helpers.o pipes.o
	$(CC) $(LDFLAGS) -o $@ $^

hijos.o: hijos.c 
	$(CC) $(CFLAGS) -o $@ -c $<

helpers.o: helpers.c
	$(CC) $(CFLAGS) -o $@ -c $<

pipes.o: pipes.c
	$(CC) $(CFLAGS) -o $@ -c $<