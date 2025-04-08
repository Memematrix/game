#include "bgame.h"
#include "btextureManager.h"
#include "bmap.h"
#include "bGameObj.h"
#include "bmusicManager.h"

bool isPlayer2 = false; //p1 is 0 and p2 is 1
bool isConfirmState = false;
bool isRotate = false;
bool hasShot = false;
bool hasReset = true;
int gameStage = 0; //stage 0 is main menu, 1 is placing ship. 2 is shooting ship, 3 is win screen
int pIndex = 0; //ship index in a set of ships
int boomTimer = -1; //time for explosion animation
int isHit = 0;
SDL_Point mousePos;
pair<double, double> trajectory;
vector<int> shipLen = {1,1,1,1,1};
vector<vector<int>> shipHP = {{1,1,1,1,1}, {1,1,1,1,1}};
vector<int> shipLeft = {5, 5};
vector<set<pair<int,int>>> usedCoordinate(2);
vector<vector<vector<pair<int,int>>>> shipCoordinate(2, vector<vector<pair<int,int>>>(5));

void shotShip();
void checkWin();
void placeShip();
void updateShipIndex();
int checkShipOverlay();


Game::Game() {}
Game::~Game() {}
SDL_Event Game::event;
SDL_Renderer* Game::renderer = nullptr;

vector<GameObj*>players;
vector<GameObj*>curMapShown;
vector<vector<GameObj*>> shipSet;
vector<vector<GameObj*>> shipSetWhite;
//Main menu
GameObj *mainTitle = nullptr, *startBtn = nullptr, *exitBtn = nullptr, *mainMenuBorder = nullptr;
//Stage 1 and 2 miscellaneous
GameObj *player1turn = nullptr, *player2turn = nullptr, *p1MapSymbol = nullptr, *p2MapSymbol = nullptr, *rotate = nullptr;
//Stage 1 ship mapping
GameObj *chooseBox = nullptr, *shipHead = nullptr, *shipBody = nullptr, *shipTail = nullptr, *shipHeadWhite = nullptr, *shipBodyWhite = nullptr, *shipTailWhite = nullptr;
//Stage 2 status update
GameObj *mainShip = nullptr, *hitWarn = nullptr, *missWarn = nullptr, *bigBoom = nullptr, *canonBall = nullptr;
//Stage 3 screen (win screen)
GameObj *p1Win = nullptr, *p2Win = nullptr;
//Confirm screen
GameObj *overlay = nullptr, *confirmScreen = nullptr, *yesConfirm = nullptr, *noConfirm = nullptr, *confirmBorder = nullptr, *backBtn = nullptr;

//Sound
Sound *backgroundMusic = nullptr, *canonSound = nullptr;
//Map
vector<Map*> playersMap;
Map *mapP1 = nullptr, *mapP2 = nullptr;

void Game::init(const char* filename, int posX, int posY, int width, int height, bool fullscreen) {
    int flags = 0;
    if(fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }
    if(SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        cout << "Subsystem initialized" << endl;
        window = SDL_CreateWindow(filename, posX, posY, width, height, flags);
        if(window) {
            cout << "Window created" << endl;
        }
        renderer = SDL_CreateRenderer(window, -1, 0);
        if(renderer) {
            SDL_SetRenderDrawColor(renderer, 96, 223, 255, 255);
            cout << "Render success" << endl;
        }
        isRunning = true;
    } else {
        isRunning = false;
    }

    if(Mix_Init(0) == 0) {
        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
        backgroundMusic = new Sound("/home/khoa/Desktop/C++game/battleship/music/themeSong.mp3");
        canonSound = new Sound("/home/khoa/Desktop/C++game/battleship/music/canonFirecut.wav");
        backgroundMusic->initBackgroundMusic();
        canonSound->initSFX();
        backgroundMusic->playMusic(-1);
    }

    mainTitle = new GameObj("/home/khoa/Desktop/C++game/battleship/image/gameName.png", 585, 250, 255, 750, 0);
    startBtn = new GameObj("/home/khoa/Desktop/C++game/battleship/image/start.png", 860, 525, 80, 200, 0);
    exitBtn = new GameObj("/home/khoa/Desktop/C++game/battleship/image/exit.png", 860, 625, 80, 200, 0);
    mainMenuBorder = new GameObj("/home/khoa/Desktop/C++game/battleship/image/mainMenuBorder.png", 860, 510, 100, 200, 100);

    player1turn = new GameObj("/home/khoa/Desktop/C++game/battleship/image/player1turn.png", 780, 50, 64, 360, 0);
    player2turn = new GameObj("/home/khoa/Desktop/C++game/battleship/image/player2turn.png", 780, 50, 64, 360, 0);
    mainShip = new GameObj("/home/khoa/Desktop/C++game/battleship/image/mainShip.png", 1620, 0, 1080, 300, 0);
    backBtn = new GameObj("/home/khoa/Desktop/C++game/battleship/image/back.png", 0, 0, 75, 200, 0);
    chooseBox = new GameObj("/home/khoa/Desktop/C++game/battleship/image/choosebox.png", MAP_START_POS_X, MAP_START_POS_Y, 32, 32, 32);

    p1MapSymbol = new GameObj("/home/khoa/Desktop/C++game/battleship/image/P1map.png", 450, 125, 75, 150, 0);
    p2MapSymbol = new GameObj("/home/khoa/Desktop/C++game/battleship/image/P2map.png", 450, 125, 75, 150, 0);
    rotate = new GameObj("/home/khoa/Desktop/C++game/battleship/image/rotate.png", 800, 825, 100, 300, 0);
    confirmScreen = new GameObj("/home/khoa/Desktop/C++game/battleship/image/confirmScreen.png", 576, 250, 432, 768, 0);    
    yesConfirm = new GameObj("/home/khoa/Desktop/C++game/battleship/image/yesConfirm.png", 700, 500, 151, 249, 0);
    noConfirm = new GameObj("/home/khoa/Desktop/C++game/battleship/image/noConfirm.png", 950, 500, 151, 249, 0);
    confirmBorder = new GameObj("/home/khoa/Desktop/C++game/battleship/image/confirmBorder.png", 720, 510, 129, 230, 240);

    shipHead = new GameObj("/home/khoa/Desktop/C++game/battleship/image/shipHead.png", MAP_START_POS_X, MAP_START_POS_Y, 32, 32, 0);
    shipTail = new GameObj("/home/khoa/Desktop/C++game/battleship/image/shipTail.png", MAP_START_POS_X, MAP_START_POS_Y, 32, 32, 0);
    shipHeadWhite = new GameObj("/home/khoa/Desktop/C++game/battleship/image/shipHeadWhite.png", MAP_START_POS_X, MAP_START_POS_Y, 32, 32, 0);
    shipTailWhite = new GameObj("/home/khoa/Desktop/C++game/battleship/image/shipTailWhite.png", MAP_START_POS_X, MAP_START_POS_Y, 32, 32, 0);

    bigBoom = new GameObj("/home/khoa/Desktop/C++game/battleship/image/bigBoom.png", 800, 120, 64, 64, 0);
    canonBall = new GameObj("/home/khoa/Desktop/C++game/battleship/image/canonBall.png", 1680, 650, 32, 32, 0);
    
    p1Win = new GameObj("/home/khoa/Desktop/C++game/battleship/image/player1Win.png", 0, 0, 1080, 1920, 0);
    p2Win = new GameObj("/home/khoa/Desktop/C++game/battleship/image/player2Win.png", 0, 0, 1080, 1920, 0);

    hitWarn = new GameObj("/home/khoa/Desktop/C++game/battleship/image/hitWarn.png", 875, 850, 75, 150, 0);
    missWarn = new GameObj("/home/khoa/Desktop/C++game/battleship/image/missWarn.png", 875, 850, 75, 150, 0);

    overlay = new GameObj("/home/khoa/Desktop/C++game/battleship/image/overlay.png", 0, 0, 1080, 1920, 0);

    players.insert(players.end(), {player1turn, player2turn});

    curMapShown.insert(curMapShown.end(), {p1MapSymbol, p2MapSymbol});

    mapP1 = new Map();
    mapP2 = new Map();

    playersMap.insert(playersMap.end(), {mapP1, mapP2});

    for(int i = 0; i < 5; ++i) {// put parts of ship into vector
        vector<GameObj*>temp;
        vector<GameObj*> tempWhite;
        temp.push_back(shipHead);
        tempWhite.push_back(shipHeadWhite);
        for(int j = 0; j < shipLen[i] - 2; ++j) {
            temp.push_back(new GameObj("/home/khoa/Desktop/C++game/battleship/image/shipBody.png", MAP_START_POS_X, MAP_START_POS_Y, 32, 32, 0));
            tempWhite.push_back(new GameObj("/home/khoa/Desktop/C++game/battleship/image/shipBodyWhite.png", MAP_START_POS_X, MAP_START_POS_Y, 32, 32, 0));
        }
        temp.push_back(shipTail);
        tempWhite.push_back(shipTailWhite);
        shipSet.push_back(temp);
        shipSetWhite.push_back(tempWhite);
        temp.clear();
        tempWhite.clear();
    }
}
void Game::eventHandler() {
    SDL_PollEvent(&event);
    switch (event.type) {
    case SDL_QUIT:
        isRunning = false;
        break;
    }
    if(isConfirmState == false){ //when not in confirm screen
        //keyboard
        if(event.type == SDL_KEYDOWN && boomTimer == -1 && !hasShot) {//won't move during shooting and exploding
            switch (event.key.keysym.sym) { //making box movement
                case SDLK_w:
                    if(gameStage != 0) {
                        chooseBox->updatePosGoUp();
                    } else {
                        if(mainMenuBorder->getPosY() > 510) {//selecting option in main menu
                            mainMenuBorder->updatePosGoUp();
                        }
                    }
                    break;
                case SDLK_s:
                    if(gameStage != 0) {
                        chooseBox->updatePosGoDown();
                    } else {
                        if(mainMenuBorder->getPosY() < 600) {//selecting option in main menu
                            mainMenuBorder->updatePosGoDown();
                        }
                    }
                    break;
                case SDLK_a:
                    chooseBox->updatePosGoLeft();
                    break;
                case SDLK_d:
                    chooseBox->updatePosGoRight();
                    break;
                case SDLK_z:
                    isRotate = !isRotate;
                    break;
                case SDLK_RETURN: //general casees when hit Enter
                    switch (gameStage) {
                    case 0:
                        if(SDL_HasIntersection(&mainMenuBorder->destRect, &startBtn->destRect)) {
                            gameStage = 1;
                        } else {
                            isConfirmState = true;
                        }
                        break;
                    case 1:
                        if(checkShipOverlay() == 0) { //when ship part not overlap
                            placeShip();
                            updateShipIndex();
                        }
                        break;
                    case 2:
                        shotShip();
                        if(isHit) {
                            canonSound->playSFX();
                        }
                        break;
                    case 3:
                        gameStage = 0;
                        hasReset = false;
                        break;
                    }
                break;
            }
        }
        //mouse
        if(event.type == SDL_MOUSEMOTION) {//reduce button opacity when hovering mouse
            SDL_GetMouseState(&mousePos.x, &mousePos.y);
            if(SDL_PointInRect(&mousePos, &startBtn->destRect)) {
                startBtn->applyTransparency(128);
            } else {
                startBtn->applyTransparency(255);
            }
            if(SDL_PointInRect(&mousePos, &exitBtn->destRect)){
                exitBtn->applyTransparency(128);
            } else {
                exitBtn->applyTransparency(255);
            }
        }
        if(event.type == SDL_MOUSEBUTTONDOWN && boomTimer == -1 && !hasShot) {//general cases when click mouse
            if(gameStage == 0) {
                SDL_GetMouseState(&mousePos.x, &mousePos.y);
                if(SDL_PointInRect(&mousePos, &startBtn->destRect)) {
                    gameStage = 1;
                }
                if(SDL_PointInRect(&mousePos, &exitBtn->destRect)){
                    isConfirmState = true;
                }
            } else if(gameStage == 1) {
                if(checkShipOverlay() == 0) { //when ship part not overlap
                    placeShip();
                    updateShipIndex();
                }
            } else if(gameStage == 2) {
                shotShip();
                if(isHit) {
                    canonSound->playSFX();
                }
            }
        }
        
    } else {//when in confirm menu
        //keyboard
        if(event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            case SDLK_d:
                if(confirmBorder->getPosX() < 950) {
                    confirmBorder->updatePosGoRight();
                }
                break;
            case SDLK_a:
                if(confirmBorder->getPosX() > 720) {
                    confirmBorder->updatePosGoLeft(); 
                } 
                break;
            case SDLK_RETURN:
                if(!SDL_HasIntersection(&confirmBorder->destRect, &noConfirm->destRect)) {
                    if(gameStage != 0) {
                        gameStage = 0;
                        hasReset = false;
                    } else {
                        isRunning = false;
                    }
                }
                isConfirmState = false;
                break;
            default:
                break;
            }
        }
        //mouse
        if(event.type == SDL_MOUSEMOTION) { //reduce button opacity when hovering mouse
            SDL_GetMouseState(&mousePos.x, &mousePos.y);
            if(SDL_PointInRect(&mousePos, &noConfirm->destRect)) {
                noConfirm->applyTransparency(128);
            } else {
                noConfirm->applyTransparency(255);
            }
            if(SDL_PointInRect(&mousePos, &yesConfirm->destRect)){
                yesConfirm->applyTransparency(128);
            } else {
                yesConfirm->applyTransparency(255);
            }
        }
        if(event.type == SDL_MOUSEBUTTONDOWN) {//general cases when click mouse
            SDL_GetMouseState(&mousePos.x, &mousePos.y);
            if(SDL_PointInRect(&mousePos, &noConfirm->destRect)) {
                isConfirmState = false;
            }
            if(SDL_PointInRect(&mousePos, &yesConfirm->destRect)){
                if(gameStage != 0) {
                    gameStage = 0;
                    hasReset = false;
                    isConfirmState = false;
                } else {
                    isRunning = false;
                }
            }
        }
    }

    if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {//press esc to open/close confirm screen
        isConfirmState = !isConfirmState;
    }
    
}
void Game::update() {
    bigBoom->destRect = chooseBox->destRect;
    //update ship head, body, tail according to choose box
    //"white set" is to show overlapping parts
    pair<int,int> iniPos = chooseBox->getMapCoordinate();
    shipSet[pIndex % 5][0]->setPos(iniPos.first, iniPos.second);
    shipSetWhite[pIndex % 5][0]->setPos(iniPos.first, iniPos.second);
    for(int i = 1; i < shipLen[pIndex % 5]; ++i) {
        iniPos = shipSet[pIndex % 5][i - 1]->getMapCoordinate();
        shipSet[pIndex % 5][i]->setPos(iniPos.first + isRotate, iniPos.second + !isRotate);
        shipSetWhite[pIndex % 5][i]->setPos(iniPos.first + isRotate, iniPos.second + !isRotate);
    }

    if(hasShot && gameStage != 3) {
        if(!SDL_HasIntersection(&chooseBox->destRect, &canonBall->destRect)) {//keep moving canon when not hit target 
            canonBall->moveDestRect(trajectory, 2);
            
        } else {
            boomTimer = 200; //after hit, got 200ms to show explosion
            canonBall->resetDestRect();
            hasShot = false;
        }
    } else {
        if(event.type == SDL_MOUSEMOTION && boomTimer == -1 && !isConfirmState) {//update choose box pos according to mouse pos
            SDL_GetMouseState(&mousePos.x, &mousePos.y);
            chooseBox->updatePosMouse(mousePos.x, mousePos.y);
        }
    }
    if(boomTimer == 0) {//check winning after explosion
        checkWin();
        playersMap[!isPlayer2]->putCheckOnMap(chooseBox->getMapCoordinate(), isHit);
        if(isHit == 2) {
            isPlayer2 = !isPlayer2;
        }
    }
}

void Game::render() {
    SDL_RenderClear(renderer);
    switch (gameStage)
    {
    case 0:
        mainTitle->renderWithRotation(0);
        startBtn->renderWithRotation(0);
        exitBtn->renderWithRotation(0);
        mainMenuBorder->renderWithRotation(0);
        break;
    case 1:
        players[isPlayer2]->renderWithRotation(0);
        playersMap[isPlayer2]->drawMap(gameStage);
        curMapShown[isPlayer2]->renderWithRotation(0);
        rotate->renderWithRotation(0);
        backBtn->renderWithRotation(0);
        chooseBox->renderWithRotation(0);
        for(int i = 0; i < shipLen[pIndex % 5]; ++i) {
            if(!playersMap[isPlayer2]->checkShipOnMap(shipSet[pIndex % 5][i]->getMapCoordinate())) {
                shipSet[pIndex % 5][i]->renderWithRotation(90 * isRotate);
            } else {
                shipSetWhite[pIndex % 5][i]->renderWithRotation(90 * isRotate);
            }
        }
        break;
    case 2:
        players[isPlayer2]->renderWithRotation(0);
        playersMap[!isPlayer2]->drawMap(gameStage);
        chooseBox->renderWithRotation(0);
        curMapShown[!isPlayer2]->renderWithRotation(0);
        backBtn->renderWithRotation(0);
        if(!SDL_HasIntersection(&canonBall->destRect, &chooseBox->destRect)) {//only show when not contact
            canonBall->renderWithRotation(0);
        }
        mainShip->renderWithRotation(0);
        if(boomTimer >= 0) { //start showing upon contact
            bigBoom->renderWithRotation(0);
            if(isHit == 1) {
                hitWarn->renderWithRotation(0);
            } else if (isHit == 2) {
                missWarn->renderWithRotation(0);
            }
            boomTimer -= 1;
        }
        break;
    case 3:
        if(isPlayer2 == false) {
            p1Win->renderWithRotation(0);
        } else {
            p2Win->renderWithRotation(0);
        }
        break;
    }
    if(isConfirmState) {
        overlay->renderWithRotation(0);
        overlay->applyTransparency(128);
        confirmScreen->renderWithRotation(0);
        yesConfirm->renderWithRotation(0);
        noConfirm->renderWithRotation(0);
        confirmBorder->renderWithRotation(0);
    }
    SDL_RenderPresent(renderer);
}

void Game::gameReset() {
    if(gameStage == 0 && !hasReset) {
        isPlayer2 = false; //p1 is 0 and p2 is 1
        isRotate = false;
        pIndex = 0;
        isHit = 0;
        hasShot = false;
        shipLen = {1,1,1,1,1};
        shipHP = {{1,1,1,1,1}, {1,1,1,1,1}};
        shipLeft = {5, 5};
        usedCoordinate.resize(2);
        shipCoordinate.resize(2, vector<vector<pair<int, int>>>(5));
        for(int i = 0; i < 2; ++i) {
            playersMap[i]->resetMap();
        }
        hasReset = true;
    }
}

void Game::clear() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    backgroundMusic->quitMusic();
    canonSound->quitSound();
    Mix_CloseAudio();
    SDL_Quit();
    cout << "Game closed" << endl;
}

int checkShipOverlay() {
    int isPlaced = 0; //check if ship part overlap
    for(int i = 0; i < shipLen[pIndex % 5]; ++i) {
        isPlaced += playersMap[isPlayer2]->checkShipOnMap(shipSet[pIndex % 5][i]->getMapCoordinate());
    }
    return isPlaced;
}

void placeShip() {
    vector<pair<int, int>> temp;
    for(int i = 0; i < shipLen[pIndex % 5]; ++i) {
        int num;
        if(i == 0) {
            num = 3;
        } else if (i == shipLen[pIndex % 5] - 1) {
            num = 7;
        } else {
            num = 5;
        }
        playersMap[isPlayer2]->putShipOnMap(shipSet[pIndex % 5][i]->getMapCoordinate(), num + isRotate);
        temp.push_back(shipSet[pIndex % 5][i]->getMapCoordinate());
    }
    shipCoordinate[isPlayer2][pIndex % 5] = temp;
}

void updateShipIndex() {
    if(pIndex == 9) { //getting ships in a set of ship
        gameStage = 2;
        isPlayer2 = false;
    } else if (pIndex == 4) {
        isPlayer2 = true;
        pIndex += 1;
    } else {
        pIndex += 1;
    }
}

void shotShip() {
    isHit = playersMap[!isPlayer2]->getHitStatus(chooseBox->getMapCoordinate());
    if(isHit) {
        trajectory = canonBall->getTrajectory(canonBall->destRect, chooseBox->destRect);
        hasShot = true;
    }
    int index = Map::shipIndex(shipCoordinate, chooseBox->getMapCoordinate(), usedCoordinate, !isPlayer2);
    if(index != -1) {
        shipHP[!isPlayer2][index] -= 1;
        if(shipHP[!isPlayer2][index] == 0) {
            shipLeft[!isPlayer2] -= 1;
        };
        cout << "Player:"<< isPlayer2 << " , Idx: " << index << " , Hp: " << shipHP[!isPlayer2][index] << ", Left: " << shipLeft[!isPlayer2] << endl;
    }
}

void checkWin() {
    if(shipLeft[!isPlayer2] == 0) {
        hasShot = false;
        boomTimer = -1;
        gameStage = 3;
    }
}
