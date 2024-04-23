#include "Grid.h"
#include <iostream>

void Grid::doStep(int x, int y, Color color) {
    if (x == -1) return;

//    if (data[x][y] == BLANK) {
//        data[x][y] = color;
//    }

    data[y][x] = color;
}


void Grid::doMove(Move move, Color color) {
    if (move.x0 == -1) return;
    data[move.y0][move.x0] = color;

    if (move.x1 == -1) return;
    data[move.y1][move.x1] = color;
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
    // grid
    std::cout << "GRID:" << std::endl;
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

    // count
    for (int type=0; type<4; type++) {
        std::cout << "COUNT " << type + 1 << ":" << std::endl;
        for (int y=0; y<GRID_SIZE; y++) {
            for (int x=0; x<GRID_SIZE; x++) {
                if (count[y][x][type] >= 0) {
                    std::cout << " ";
                }
                std::cout << count[y][x][type] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
}


