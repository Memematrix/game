#include "bgame.h"
using namespace std;
class Map {
public:
    Map();
    ~Map();
    void loadMapStage1(vector<vector<int>>& map);
    void loadMapStage2(vector<vector<int>>& map);
    void drawMap(int stage);
    int getHitStatus(const pair<int, int>& index);
    void putCheckOnMap(const pair<int, int>& index, int num);
    void putShipOnMap(const pair<int, int>& index, int num);
    bool checkShipOnMap(const pair<int, int>& index);
    static int shipIndex(const vector<vector<vector<pair<int,int>>>>& shipCoordinate, const pair<int,int>& index, vector<set<pair<int,int>>>& usedCoordinate, bool isPlayer2);
    void resetMap();
    private:
    vector<vector<int>> mapStage1 = vector<vector<int>>(SIZE, vector<int>(SIZE, 0));
    vector<vector<int>> mapStage2 = vector<vector<int>>(SIZE, vector<int>(SIZE, 0));
    SDL_Rect srcR, destR;
    SDL_Texture *notHit;
    SDL_Texture *hit;
    SDL_Texture *miss;
    SDL_Texture *shipHead = nullptr, *shipHead90CW = nullptr, *shipBody = nullptr, *shipTail = nullptr, *shipTail90CW = nullptr;
};