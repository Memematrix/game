#ifndef bgame_h
#define bgame_h
#define SIZE 15
#define MAP_START_POS_X (960/32 - SIZE/2)*32 //auto put map in center screen
#define MAP_START_POS_Y 160
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <vector>
#include <set>
using namespace std;
class Game {
public:
    Game();
    ~Game();
    void init(const char* title, int posX, int posY, int width, int height, bool fullScreen);
    void eventHandler();
    void update(); //update object
    void render(); //render screen
    void clear(); //clear screen
    bool running() {
        return isRunning;
    } //check if game running
    void gameReset();
    static SDL_Renderer *renderer;
    static SDL_Event event;
    
    private:
    bool hasReach = false;
    bool isRunning;
    SDL_Window *window = nullptr;
};

#endif