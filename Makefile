CC=g++
MW=i686-w64-mingw32-g++

CFLAGS=-std=c++17 
LFLAGS=`sdl2-config --libs ` -lSDL2_ttf -lSDL2 -lSDL2_image

#.PHONY build run clean window

build: main.cpp
	$(CC) main.cpp words.cpp save.cpp -o program $(CFLAGS) $(LFLAGS)

run: main.cpp
	make build && ./program

clean:
	rm -f program program.exe
	rm -f *.o
	rm save.txt

window: main.cpp
	$(MW) main.cpp words.cpp -o program $(CFLAGS) -ISDL2-2.0.18/include -ISDL2_ttf-2.0.15/include -lmingw32 -lSDL2_ttf -lSDL2 -LSDL2_ttf-2.0.15/lib -Wl,-rpath,SDL2_ttf-2.0.15/lib
	
