#include "bGameObj.h"
#include "btextureManager.h"
GameObj::GameObj(const char* textureSheet, int x, int y, int h, int w, int spd) {
    objTexture = textureManager::loadTexture(textureSheet);
    destRect.w = w;
    destRect.h = h;
    destRect.x = x;
    destRect.y = y;
    speed = spd;
    posX = x;
    posY = y;
    width = w;
    height = h;
    velocityX = 0;
    velocityY = 0;
}
void GameObj::updatePosMouse(const int& xMouse, const int& yMouse) {
    destRect.y = (yMouse >= 160 && yMouse < (SIZE+5)*32) ? (yMouse/32 * 32) : destRect.y;
    destRect.x = (xMouse >= MAP_START_POS_X && xMouse < (SIZE*32 + MAP_START_POS_X)) ? (xMouse/32 * 32) : destRect.x;
}
void GameObj::updatePosGoUp() {
    if(destRect.y - speed >= posY) {
        destRect.y -= speed;
    } else {
        destRect.y = posY + height*SIZE - height;
    }
}
void GameObj::updatePosGoDown() {
    if(destRect.y + speed < posY + height*SIZE) {
        destRect.y += speed;
    } else {
        destRect.y = posY;
    }
}
void GameObj::updatePosGoLeft() {
    if(destRect.x - speed >= posX) {
        destRect.x -= speed;
    } else {
        destRect.x = posX + width*SIZE - width;
    }
}
void GameObj::updatePosGoRight() {
    if(destRect.x + speed < posX + width*SIZE) {
        destRect.x += speed;
    } else {
        destRect.x = posX;
    }
}
pair<int,int> GameObj::getMapCoordinate() {
    pair<int,int> index;
    index.first = (destRect.y - posY)/height;
    index.second = (destRect.x - posX)/width;
    return index;
}

void GameObj::setPos(const int &indexY, const int &indexX) {
    destRect.y = posY + indexY * height;
    destRect.x = posX + indexX * width;
}
int GameObj::getPosX() {
    return destRect.x;
}
int GameObj::getPosY() {
    return destRect.y;
}
void GameObj::renderWithRotation(const double &angle) {
    SDL_RenderCopyEx(Game::renderer, objTexture, NULL, &destRect, angle, NULL, SDL_FLIP_NONE);
}


void GameObj::applyTransparency(Uint8 alpha) {
    SDL_SetTextureBlendMode(objTexture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(objTexture, alpha);
}

pair<double, double> GameObj::getTrajectory(SDL_Rect dest, SDL_Rect start) {
    double a, b;
    a = (double)(dest.y - start.y)/(double)(dest.x - start.x);
    b = dest.y - (dest.x * a);
    return {a, b};
}


void GameObj::moveDestRect(const pair<double, double>& traject, const int &speed) {
    destRect.x -= speed;
    destRect.y = traject.first * destRect.x + traject.second;
}

void GameObj::resetDestRect() {
    destRect.x = posX;
    destRect.y = posY;
}
