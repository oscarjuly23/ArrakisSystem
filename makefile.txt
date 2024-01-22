all:
read.o: ./SourceFiles/All/read.c ./HeaderFiles/read.h
	gcc -c ./SourceFiles/All/read.c

all: Fremen
Fremen.o: ./SourceFiles/Fremen/Fremen.c
	gcc -c ./SourceFiles/Fremen/Fremen.c
actionHandler.o: ./SourceFiles/Fremen/actionHandler.c ./HeaderFiles/actionHandler.h
	gcc -c ./SourceFiles/Fremen/actionHandler.c
client.o: ./SourceFiles/Fremen/client.c ./HeaderFiles/client.h
	gcc -c ./SourceFiles/Fremen/client.c
Fremen: Fremen.o actionHandler.o client.o read.o
	gcc Fremen.o actionHandler.o client.o read.o -o Fremen -Wall -Wextra

all: Atreides
Atreides.o: ./SourceFiles/Atreides/Atreides.c
	gcc -c ./SourceFiles/Atreides/Atreides.c
server.o: ./SourceFiles/Atreides/server.c ./HeaderFiles/server.h
	gcc -c ./SourceFiles/Atreides/server.c
thread.o: ./SourceFiles/Atreides/thread.c ./HeaderFiles/thread.h
	gcc -c ./SourceFiles/Atreides/thread.c
user.o: ./SourceFiles/Atreides/user.c ./HeaderFiles/user.h
		gcc -c ./SourceFiles/Atreides/user.c

Atreides: Atreides.o server.o thread.o user.o read.o
	gcc Atreides.o server.o thread.o user.o read.o -lpthread -o Atreides -Wall -Wextra

all: Harkonen
Harkonen.o: ./SourceFiles/Harkonen/Harkonen.c
	gcc -c ./SourceFiles/Harkonen/Harkonen.c

Harkonen: Harkonen.o
	gcc Harkonen.o -o Harkonen -Wall -Wextra

.PHONY: clean
clean:
	rm -rf *.o Fremen Atreides Harkonen
