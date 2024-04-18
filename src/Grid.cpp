#include "Grid.h"
#include <iostream>

void Grid::doStep(int x, int y, Color color) {
    if (x == -1) return;

//    if (data[x][y] == BLANK) {
//        data[x][y] = color;
//    }

    data[x][y] = color;
}


void Grid::undoStep(int x, int y) {
    data[x][y] = BLANK;
}


std::vector<Step> Grid::getAll() {
    std::vector<Step> availableSteps;

    for (int x=0; x<GRID_SIZE; x++) {
        for (int y=0; y<GRID_SIZE; y++) {
            if (data[x][y] != BLANK) continue;
            availableSteps.emplace_back(x, y);
        }
    }

    return availableSteps;
}


void Grid::output() {
    for (int x=0; x<GRID_SIZE; x++) {
        for (int y=0; y<GRID_SIZE; y++) {
            if (data[x][y] == -1) {
                std::cout << "w ";
            } else if (data[x][y] == 1) {
                std::cout << "b ";
            } else {
                std::cout << data[x][y] << " ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}