.PHONY: all clean 
CC=clang
CFLAGS=-c -std=c89 -Wall -Wextra -Wpedantic -Weverything
TARGET=yoda

# найти еще флаги

all: clean $(TARGET)

main.o: main.c
	$(CC) $(CFLAGS) $^ -o $@

stack.o: stack.c
	$(CC) $(CFLAGS) $^ -o $@

$(TARGET): main.o stack.o
	$(CC) $^ -o $@

clean:
	rm -rf *.o yoda
