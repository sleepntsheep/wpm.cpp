#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <array>
#include <charconv>
#include <vector>
#include <tuple>
#include <utility>
#include "main.h"
#include "words.h"

SDL_Renderer* renderer;
SDL_Window* window;
TTF_Font* font;
SDL_Color color;
bool running;
SDL_Color White = {255, 255, 255};
SDL_Color Black = {0, 0, 0};
unsigned int tick1 = SDL_GetTicks();
unsigned int tick2 = SDL_GetTicks();
double delta;
SDL_Rect statusBar = {0, HEIGHT-BARHEIGHT, WIDTH, BARHEIGHT};

std::string inputStr = "";
int score = 0;
int hp = HP;
float wpm;

class Word {
    public:
        int x, y;
        std::string text;
        //Word(std::string t, int x1, int y1)
};

std::vector<Word> wordsList;

void update() {
    if (hp <= 0) {
        running = false;
        std::cout << "You lost, wpm: " + std::to_string(wpm) <<std::endl;
    }

    wpm = (float)score/(float)((float)tick2/60000.0);
    while (wordsList.size() < 15) {
        Word temp;
        temp.text = getLine();
        temp.x = randomNumber(0, 150);
        temp.y = randomNumber(0, HEIGHT - 100);
        wordsList.push_back(temp);
    }

    std::vector<Word> tempList;
    for (auto word : wordsList) {
        if (word.x > WIDTH) {
            hp--;
            continue;
        }
        word.x += SPEED;
        tempList.push_back(word);
    }
    wordsList = tempList;
}

void checkInput() {
    std::vector<Word> tempList;
    bool found = false;
    for (auto word: wordsList) {
        if (word.text == inputStr && !found) {
            found = true;
            score++;
            continue;
        }
        tempList.push_back(word);
    }
    wordsList = tempList;

    inputStr = "";
}

void input() {
    SDL_Event e;
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);
    SDL_StartTextInput();
    while(SDL_PollEvent(&e)) {
        switch ( e.type ) {
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym)
                {
                    case SDLK_RETURN:
                        checkInput();
                        break;
                    case SDLK_BACKSPACE:
                        inputStr.pop_back();
                        break;
                    default:
                        break;
                }
                break;
            case SDL_QUIT:
                running = false;
                break;
            case SDL_TEXTINPUT:
                if (*e.text.text == ' ') {
                    checkInput();
                }
                else {
                    inputStr += e.text.text;
                    break;
                }
            default: break;
        }
    }
}

void write(std::string str, int x, int y) {
    SDL_Surface *surface;
    SDL_Texture *texture;

    TTF_Font* font = TTF_OpenFont(FONT_PATH, FONT_SIZE);
    if (!font)
        std::cout << "Couldn't find/init open ttf font >> " << TTF_GetError() << std::endl;
    surface = TTF_RenderText_Solid(font, str.c_str(), Black);
    TTF_CloseFont(font);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    int text_width = surface->w;
    int text_height = surface->h;
    SDL_Rect message_rect = { x, y, text_width, text_height };
    SDL_RenderCopy(renderer, texture, NULL, &message_rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void render() {
    SDL_SetRenderDrawColor(renderer, 0xFB, 0xF1, 0xC7, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 255);
    SDL_RenderFillRect(renderer, &statusBar);

    for (auto word : wordsList) {
        write(word.text, word.x, word.y);
    }

    write("["+inputStr+"]"+" | Score: "+std::to_string(score)+" | wpm: "+std::to_string(wpm)+" | HP: "+std::to_string(hp), 30, HEIGHT - BARHEIGHT + 10);

    SDL_RenderPresent(renderer);
}

int main() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        std::cout << "Failed at SDL_Init()" <<  std::endl;
    if (SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer) < 0)
        std::cout << "Failed at SDL_CreateWindowAndRenderer()" << std::endl;
    if (TTF_Init() < 0)
        std::cout << "Failed at TTF_Init" << TTF_GetError() << std::endl;
    running = true;

    while (running) {
        Uint64 start = SDL_GetPerformanceCounter();
        tick2 = SDL_GetTicks();

        delta = tick2 - tick1;

        if (delta > 1000.0/(float)FPS) {
            tick1 = SDL_GetTicks();
            update();
            input();
            render();
        }

    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
