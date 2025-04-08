#include "bmap.h"
#include "btextureManager.h"

Map::Map() {
    notHit = textureManager::loadTexture("/home/khoa/Desktop/C++game/battleship/image/notHit.png");
    miss = textureManager::loadTexture("/home/khoa/Desktop/C++game/battleship/image/hit.png");
    hit = textureManager::loadTexture("/home/khoa/Desktop/C++game/battleship/image/miss.png");
    shipHead = textureManager::loadTexture("/home/khoa/Desktop/C++game/battleship/image/shipHead.png");
    shipHead90CW = textureManager::loadTexture("/home/khoa/Desktop/C++game/battleship/image/shipHead90CW.png");
    shipBody = textureManager::loadTexture("/home/khoa/Desktop/C++game/battleship/image/shipBody.png");
    shipTail = textureManager::loadTexture("/home/khoa/Desktop/C++game/battleship/image/shipTail.png");
    shipTail90CW = textureManager::loadTexture("/home/khoa/Desktop/C++game/battleship/image/shipTail90CW.png");


    srcR.x = srcR.y = 0;
    srcR.w = srcR.h = 32;

    destR.x = destR.y = 0;
    destR.w = destR.h = 32;
}

void Map::drawMap(int stage) {
    int type = 0;
    for(int i = 0; i < SIZE; ++i) {
        for(int j = 0; j < SIZE; ++j) {
            if(stage == 1) {
                type = mapStage1[i][j];
            } else {
                type = mapStage2[i][j];
            }
            destR.y = 32*(i + MAP_START_POS_Y/32);
            destR.x = 32*(j + MAP_START_POS_X/32);
            switch (type)
            {
            case 0:
                textureManager::Draw(notHit, srcR, destR);
                break;
            case 1:
                textureManager::Draw(hit, srcR, destR);
                break;
            case 2:
                textureManager::Draw(miss, srcR, destR);
                break;
            case 3:
                textureManager::Draw(shipHead, srcR, destR);
                break;
            case 4:
                textureManager::Draw(shipHead90CW, srcR, destR);
                break;
            case 5:
                textureManager::Draw(shipBody, srcR, destR);
                break;
            case 6:
                textureManager::Draw(shipBody, srcR, destR);
                break;
            case 7:
                textureManager::Draw(shipTail, srcR, destR);
                break;
            case 8:
                textureManager::Draw(shipTail90CW, srcR, destR);
                break;
            default:
                break;
            }
        }
    }
}
int Map::getHitStatus(const pair<int, int>& index) {
    if(mapStage2[index.first][index.second] == 0) {
        if(mapStage1[index.first][index.second] >= 3) {
            return 1;
        } else if(mapStage1[index.first][index.second] == 0) {
            return 2;
        }
    }
    return 0;
}
void Map::putCheckOnMap(const pair<int, int>& index, int num) {
    mapStage2[index.first][index.second] = num;
}
bool Map::checkShipOnMap(const pair<int, int>& index) {
    if(mapStage1[index.first][index.second] != 0 || index.first >= SIZE || index.second >= SIZE || index.first < 0 || index.second < 0) {
        return true;
    }
    return false;
}

void Map::putShipOnMap(const pair<int, int>& index, int num) {
    mapStage1[index.first][index.second] = num;
}

int Map::shipIndex(const vector<vector<vector<pair<int,int>>>>& shipCoordinate, const pair<int,int>& index, vector<set<pair<int,int>>>& usedCoordinate, bool isPlayer2) {
    for(int i = 0; i < shipCoordinate[isPlayer2].size(); ++i) {
        for(int j = 0; j < shipCoordinate[isPlayer2][i].size(); ++j) {
            if(index == shipCoordinate[isPlayer2][i][j]) {
                if(usedCoordinate[isPlayer2].find(index) != usedCoordinate[isPlayer2].end()) {
                    return -1;
                }
                usedCoordinate[isPlayer2].insert(index);
                return i;
            }
        }
    }
    cout << "Ship index not found" << endl;
    return -1;
}

void Map::resetMap() {
    for(int i = 0; i < SIZE; ++i) {
        for(int j = 0; j < SIZE; ++j) {
            mapStage1[i][j] = 0;
            mapStage2[i][j] = 0;
        }
    }
}


