#include "Grid.h"
#include <iostream>

void Grid::doStep(int x, int y, Color color) {
    if (x == -1) return;

//    if (data[x][y] == BLANK) {
//        data[x][y] = color;
//    }

    data[y][x] = color;
}


void Grid::undoStep(int x, int y) {
    data[y][x] = BLANK;
}


std::vector<Step> Grid::getAll() {
    std::vector<Step> availableSteps;

    for (int y=0; y<GRID_SIZE; y++) {
        for (int x=0; x<GRID_SIZE; x++) {
            if (data[y][x] != BLANK) continue;
            availableSteps.emplace_back(x, y);
        }
    }

    return availableSteps;
}


void Grid::output() {
    for (int y=0; y<GRID_SIZE; y++) {
        for (int x=0; x<GRID_SIZE; x++) {
            if (data[y][x] == -1) {
                std::cout << "w ";
            } else if (data[y][x] == 1) {
                std::cout << "b ";
            } else {
                std::cout << data[y][x] << " ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}