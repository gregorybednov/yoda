.PHONY: all clean 
CC=clang
CFLAGS=-c -std=c89 -Wall -Wextra -Wpedantic -Weverything
TARGET=yoda

# найти еще флаги

all: $(TARGET)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

stack.o: stack.c
	$(CC) $(CFLAGS) -c stack.c -o stack.o

tritmemory.o: tritmemory.c
	$(CC) $(CFLAGS) -c tritmemory.c -o tritmemory.o

node.o: node.c
	$(CC) $(CFLAGS) -c node.o node.c

$(TARGET): main.o stack.o tritmemory.o
	$(CC) main.o stack.o tritmemory.o -o $(TARGET)

clean:
	rm -rf *.o yoda
