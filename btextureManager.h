#include "bgame.h"
class textureManager {
public:
    static SDL_Texture *loadTexture(const char* filename);
    static void Draw(SDL_Texture *tex, SDL_Rect src, SDL_Rect dest);
};
