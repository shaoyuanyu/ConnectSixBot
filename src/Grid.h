//
// ysy
// 2024/3/29
//

#ifndef CONNECT_SIX_BOT_GRID_H
#define CONNECT_SIX_BOT_GRID_H


#include "GameConfig.h"

struct Grid {
    Color grid[GRID_SIZE][GRID_SIZE] = { BLANK };
    int weight[GRID_SIZE][GRID_SIZE] = { 0 };

    void doStep(int x, int y, Color color);

    inline void addWeight(int x, int y);

    void output();
};


#endif //CONNECT_SIX_BOT_GRID_H
