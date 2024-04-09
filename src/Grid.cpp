//
// ysy
// 2024/3/29
//

#include "Grid.h"
#include <iostream>

void Grid::doStep(int x, int y, Color color) {
    if (x == -1) return;

    if (grid[x][y] == BLANK) {
        grid[x][y] = color;

        // 内层
        addWeight(x-1, y, 2);
        addWeight(x+1, y, 2);
        addWeight(x, y-1, 2);
        addWeight(x, y+1, 2);
        addWeight(x-1, y-1, 2);
        addWeight(x-1, y+1, 2);
        addWeight(x+1, y-1, 2);
        addWeight(x+1, y+1, 2);
        // 外层
        addWeight(x-2, y, 1);
        addWeight(x+2, y, 1);
        addWeight(x, y-2, 1);
        addWeight(x, y+2, 1);
        addWeight(x-2, y-2, 1);
        addWeight(x-2, y+2, 1);
        addWeight(x+2, y-2, 1);
        addWeight(x+2, y+2, 1);
    }
}

inline void Grid::addWeight(int x, int y, int addition) {
    if (x<0 || x>=GRID_SIZE || y<0 || y>=GRID_SIZE) return;
    if (weight[x][y] >= 2) return; // 权重上限

    weight[x][y] += addition;

    if (weight[x][y] > 2) weight[x][y] = 2;
}

void Grid::output() {
    for (int x=0; x<GRID_SIZE; x++) {
        for (int y=0; y<GRID_SIZE; y++) {
            std::cout << grid[x][y] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    for (int x=0; x<GRID_SIZE; x++) {
        for (int y=0; y<GRID_SIZE; y++) {
            std::cout << weight[x][y] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}