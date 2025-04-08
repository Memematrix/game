#include "bgame.h"
Game *game = nullptr;
int main(int argc, const char *argv[]) {
    int FPS = 60;
    int frameDelay = 1000/FPS;
    int frameStart, frameTime;

    game = new Game();
    game->init("Battle Ship", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, false);
    while(game->running()) {
        frameStart = SDL_GetTicks();
        game->eventHandler();
        game->update();
        game->render();
        game->gameReset();
        frameTime = SDL_GetTicks() - frameStart;

    }
    game->clear();
    delete game;
    return 0;
}
