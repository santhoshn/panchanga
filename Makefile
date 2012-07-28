#Makefile

CC= gcc
CFLAGS= -g

panchanga: plib.o panchanga.o
	$(CC) $^ -lm -o $@

clean:
	rm -f *.o panchanga 
