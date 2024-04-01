//
// ysy
// 2024/3/29
//

#ifndef CONNECT_SIX_BOT_GRID_H
#define CONNECT_SIX_BOT_GRID_H


#include "GameConfig.h"

class Grid {
private:
    BotColor grid[GRID_SIZE][GRID_SIZE] = { blank };

public:
    void doStep(int x, int y, BotColor color);
};


#endif //CONNECT_SIX_BOT_GRID_H
