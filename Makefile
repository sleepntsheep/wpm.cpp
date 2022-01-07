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

wasm: main.cpp
	emcc --shell-file template.html main.cpp words.cpp save.cpp -o index.html -s USE_SDL=2 -s USE_SDL_TTF=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='[png]' --embed-file resource -use-preload-plugins
