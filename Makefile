CC=g++
CFLAGS=-std=c++17 -o program main.cpp words.cpp -lSDL2 -lSDL2_ttf -I .

build: main.cpp
	$(CC) $(CFLAGS)

run: main.cpp
	$(CC) $(CFLAGS) && ./program

debug: main.cpp
	$(CC) -ggdb $(CFLAGS)
