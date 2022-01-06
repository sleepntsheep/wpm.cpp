#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <array>
#include <charconv>
#include <vector>
#include <tuple>
#include <utility>
#include "main.h"
#include "words.h"
#include "save.h"

SDL_Renderer* renderer;
SDL_Window* window;
TTF_Font* font;
SDL_Color color;
SDL_Color White = {255, 255, 255};
SDL_Color Red = {255, 0, 0};
SDL_Color Black = {0, 0, 0};
SDL_Texture* imgTexture;
unsigned int tick1 = SDL_GetTicks();
unsigned int tick2 = SDL_GetTicks();
bool running;
double delta;
SDL_Rect statusBar = {0, HEIGHT-BARHEIGHT, WIDTH, BARHEIGHT};

std::string inputStr = "";
std::string gameState = "intro";
int speed = SPEED;
int score = 0;
int hp = HP;
float wpm;

class Word {
    public:
        int x, y;
        std::string text;
        Word(int x, int y, std::string text) {
            this->x = x;
            this->y = y;
            this->text = text;
        }
};

std::vector<Word> wordsList;

void update() {
    if (hp <= 0) {
        running = false;
        save(wpm, (float)tick2/1000.0);
        std::cout << "You lost, wpm: " + std::to_string(wpm) <<std::endl;
    }

    wpm = (float)score/(float)((float)tick2/60000.0);
    speed += ACC;

    while (wordsList.size() < 15) {
        Word temp(randomNumber(0, 150), randomNumber(0, HEIGHT - 100), getLine());
        wordsList.push_back(temp);
    }

    std::vector<Word> tempList;
    for (auto word : wordsList) {
        if (word.x > WIDTH) {
            hp--;
            continue;
        }
        word.x += floor((float)SPEED/1000.0);
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
                        if (inputStr.length() > 0)
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

void write(std::string str, int x, int y, SDL_Color color, bool colorTyped = false) {
    SDL_Surface *surface;
    SDL_Texture *texture;

    TTF_Font* font = TTF_OpenFont(FONT_PATH, FONT_SIZE);
    if (!font)
        std::cout << "Couldn't find/init open ttf font >> " << TTF_GetError() << std::endl;
    int prefix_width = 0,
        prefix_height = 0;
    if (colorTyped && inputStr != "" && inputStr != str) {
        if (str.rfind(inputStr, 0) == 0) {
            str = str.substr(inputStr.length());
            surface = TTF_RenderText_Solid(font, inputStr.c_str(), Red);
            texture = SDL_CreateTextureFromSurface(renderer, surface);
            prefix_width = surface->w;
            prefix_height = surface->h;
            SDL_Rect message_rect = { x, y, prefix_width, prefix_height };
            SDL_RenderCopy(renderer, texture, NULL, &message_rect);
            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);
        }
    }
    if (inputStr == str)
        color = Red;
    surface = TTF_RenderText_Solid(font, str.c_str(), color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    int text_width = surface->w;
    int text_height = surface->h;
    SDL_Rect message_rect = { x+prefix_width, y, text_width, text_height };
    SDL_RenderCopy(renderer, texture, NULL, &message_rect);

    TTF_CloseFont(font);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void render() {
    SDL_SetRenderDrawColor(renderer, 0xFB, 0xF1, 0xC7, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, imgTexture, NULL, NULL);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 255);
    SDL_RenderFillRect(renderer, &statusBar);

    for (auto word : wordsList) {
        write(word.text, word.x, word.y, White, true);
    }

    write("["+inputStr+"]"+" | Score: "+std::to_string(score)+" | wpm: "+std::to_string(wpm)+" | HP: "+std::to_string(hp), 30, HEIGHT - BARHEIGHT + 10, Black);

    SDL_RenderPresent(renderer);
}

int main() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        std::cout << "Failed at SDL_Init()" <<  std::endl;
    if (SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer) < 0)
        std::cout << "Failed at SDL_CreateWindowAndRenderer()" << std::endl;
    if (TTF_Init() < 0)
        std::cout << "Failed at TTF_Init" << TTF_GetError() << std::endl;
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
        std::cout << "Failed at IMG_Init" << IMG_GetError() << std::endl;

    running = true;

    SDL_SetWindowTitle(window, "WPM");

    SDL_Surface* image = IMG_Load( BG_PATH );

    imgTexture = SDL_CreateTextureFromSurface(renderer, image);

    SDL_FreeSurface(image);
    
    if (!image) {
        std::cout << "Failed at IMG_Load" << IMG_GetError() << std::endl;
    }

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

    return 0;
}
