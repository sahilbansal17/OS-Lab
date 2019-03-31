FLAGS = -w -pthread

all: dekker peterson lamport_bakery

clean:
	rm -f dekker peterson lamport_bakery

dekker: dekker.c
	gcc -o dekker dekker.c

peterson: peterson.c
	gcc -o peterson peterson.c

lamport_bakery: lamport_bakery.c
	gcc -o lamport_bakery lamport_bakery.c $(FLAGS)
