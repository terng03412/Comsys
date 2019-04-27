# this is a collection of files working on thread, pthread and multiple
# processes
# aug-16-2000
CC   = gcc
G    = -g
EXE  = test unixmp pt mutex pandc pcsem pcno mutex-no bank
LIB  = -lpthread -lrt
all: test unixmp pt mutex pandc pcsem pcno mutex-no bank
bank: bank.o
	$(CC) -o bank bank.o $(LIB)
bank.o: bank.c
	$(CC) $G -c bank.c
pcno: p-c-nocord.o
	$(CC) -o pcno p-c-nocord.o $(LIB)
p-c-nocord.o: p-c-nocord.c
	$(CC) $G -c p-c-nocord.c
pcsem: p-c-sem.o
	$(CC) -o pcsem p-c-sem.o $(LIB)
p-c-sem.o: p-c-sem.c
	$(CC) $G -c p-c-sem.c
pandc: producer-consumer.o
	$(CC) -o pandc producer-consumer.o $(LIB)
producer-consumer.o: producer-consumer.c
	$(CC) $G -c producer-consumer.c
test: test.o
	$(CC) -o test $G test.o $(LIB)
test.o: test.c
	$(CC) $G -c test.c
unixmp: unixmp.o
	$(CC) -o unixmp $G unixmp.o
unixmp.o: unixmp.c
	$(CC) $G -c unixmp.c
pt: pthread.o
	$(CC) $G -o pt pthread.o $(LIB)
pthread.o: pthread.c
	$(CC) $G -c pthread.c
mutex: mutex.o
	$(CC) $G -o mutex mutex.o $(LIB)
mutex.o: mutex.c
	$(CC) $G -c mutex.c
mutex-no: mutex-no.o
	$(CC) $G -o mutex-no mutex-no.o $(LIB)
mutex-no.o: mutex-no.c
	$(CC) $G -c mutex-no.c
clean:
	/bin/rm -f *.o *~ core $(EXE)
