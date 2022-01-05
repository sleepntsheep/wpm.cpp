CC=g++
MW=i686-w64-mingw32-g++

CFLAGS=-std=c++17 
LFLAGS=`sdl2-config --libs -lSDL2_ttf -lSDL2` -lSDL2_ttf

build: main.cpp
	$(CC) main.cpp words.cpp -o program $(CFLAGS) $(LFLAGS)

run: main.cpp
	make build && ./program

window: main.cpp
	$(MW) main.cpp words.cpp -o program $(CFLAGS) -ISDL2/include -ISDL2_ttf/include -lmingw32 -lSDL2_ttf -lSDL2 -LSDL2_ttf/lib -Wl,-rpath,SDL2_ttf/lib
