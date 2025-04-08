#include "bgame.h"
class GameObj {
public:
    GameObj(const char *textureSheet, int posX, int posY, int h, int w, int spd);
    ~GameObj();
    void updatePosMouse(const int& xMouse, const int& yMouse);
    void updatePosGoUp();
    void updatePosGoDown();
    void updatePosGoLeft();
    void updatePosGoRight();
    void renderWithRotation(const double &angle);
    pair<int,int> getMapCoordinate();
    void setPos(const int &indexX, const int &indexY);
    void applyTransparency(Uint8 alpha);
    void moveDestRect(const pair<double, double>& traject, const int& speed);
    void resetDestRect();
    pair<double, double> getTrajectory(SDL_Rect dest, SDL_Rect start);
    int getPosX();
    int getPosY();
    int velocityY;
    int velocityX;
    SDL_Rect destRect;
private:
    int speed;
    int posX;
    int posY;
    int width;
    int height;

    SDL_Texture *objTexture;
};
