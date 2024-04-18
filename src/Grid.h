//
// 棋盘类
//

#ifndef CONNECT_SIX_BOT_GRID_H
#define CONNECT_SIX_BOT_GRID_H


#include <vector>
#include "GameConfig.h"

struct Grid {
    Color data[GRID_SIZE][GRID_SIZE] = { BLANK }; // data[y][x]

    void doStep(int x, int y, Color color);
    void undoStep(int x, int y);
    std::vector<Step> getAll();
    void output();
};


#endif //CONNECT_SIX_BOT_GRID_H
