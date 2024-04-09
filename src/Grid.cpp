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

        addWeight(x-1, y);
        addWeight(x+1, y);
        addWeight(x, y-1);
        addWeight(x, y+1);
        addWeight(x-1, y-1);
        addWeight(x-1, y+1);
        addWeight(x+1, y-1);
        addWeight(x+1, y+1);
    }
}

inline void Grid::addWeight(int x, int y) {
    if (x<0 || x>=GRID_SIZE || y<0 || y>=GRID_SIZE) return;
    if (weight[x][y] == 2) return;

    weight[x][y]++;
}

void Grid::output() {
    for (int x=0; x<GRID_SIZE; x++) {
        for (int y=0; y<GRID_SIZE; y++) {
            std::cout << grid[x][y] << " ";
        }
        std::cout << std::endl;
    }
}