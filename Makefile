CC=g++
CFLAGS=-std=c++11

all: program1

program1: main.cpp
	$(CC) $(CFLAGS) main.cpp -o program1

clean:
	rm -f program1 output.txt