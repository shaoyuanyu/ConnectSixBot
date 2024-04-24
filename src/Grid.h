//
// 棋盘类
//

#ifndef CONNECT_SIX_BOT_GRID_H
#define CONNECT_SIX_BOT_GRID_H


#include <vector>
#include "GameConfig.h"

struct Grid {
    Color data[GRID_SIZE][GRID_SIZE] = { BLANK }; // data[y][x]
    int road[GRID_SIZE][GRID_SIZE][4] = { 0 }; // road[y][x][type]，存放type型扫描时以(x,y)为起点的6格路内的纯色数

    void doStep(int x, int y, Color color);
    void doMove(Move move, Color color);

    void updateRoad(int x, int y, Color color);
    void addToRoad(int x, int y, int type, int weight);

    void undoStep(int x, int y);

    std::vector<Step> getAll();

    void output();
};


#endif //CONNECT_SIX_BOT_GRID_H
