//
// ysy
// 2024/3/29
//

#include "Grid.h"

void Grid::doStep(int x, int y, BotColor color) {
    if (x == -1) return;

    if (grid[x][y] == blank) {
        grid[x][y] = color;
    }
}

