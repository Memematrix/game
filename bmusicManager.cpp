#include "bmusicManager.h"
Sound::Sound(const char* filename) {
    filedir = filename;
}
void Sound::initSFX() {
    sfx = Mix_LoadWAV(filedir);
    if (sfx == nullptr) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Could not load sound! SDL_mixer Error: %s", Mix_GetError());
        return;
    }
}
void Sound::initBackgroundMusic() {
    music = Mix_LoadMUS(filedir);
    if (music == nullptr) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Could not load music! SDL_mixer Error: %s", Mix_GetError());
        return;
    }
}
void Sound::playMusic(int loop) {
    Mix_PlayMusic(music, loop);
}
void Sound::quitMusic() {
    Mix_FreeMusic(music);
}
void Sound::quitSound() {
    Mix_FreeChunk(sfx);
}

void Sound::playSFX() {
    Mix_PlayChannel(-1, sfx, 0);
}
