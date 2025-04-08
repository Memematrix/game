#include "SDL_mixer.h"
#include "SDL.h"
class Sound {
    public:
    Sound (const char* filename);
    void initSFX();
    void initBackgroundMusic();
    void playMusic(int loop);
    void playSFX();
    void quitMusic();
    void quitSound();
    private:
    const char* filedir;
    Mix_Music *music = nullptr;
    Mix_Chunk* sfx = nullptr;
};
