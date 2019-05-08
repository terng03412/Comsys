
FLAGS = -Wall -pthread
INCLUDES = ./include

all: cv1 client
clean:
	rm -f cv1 client

cv1: cv1.c
	gcc -I $(INCLUDES) -o cv1 cv1.c $(FLAGS)
	
client: sbuf.c client.c
	gcc -I $(INCLUDES) -o client sbuf.c client.c $(FLAGS)
