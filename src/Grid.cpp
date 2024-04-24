#include "Grid.h"
#include <iostream>

void Grid::doStep(int x, int y, Color color) {
    if (x == -1) return;
    if (data[y][x] != BLANK) return;

    data[y][x] = color;
    updateRoad(x, y, color);
}


void Grid::doMove(Move move, Color color) {
    if (move.x0 == -1) return;
    data[move.y0][move.x0] = color;
    updateRoad(move.x0, move.y0, color);

    if (move.x1 == -1) return;
    data[move.y1][move.x1] = color;
    updateRoad(move.x1, move.y1, color);
}


void Grid::updateRoad(int x, int y, Color color) {
    int weight = color;
    if (color == WHITE) {
        weight = 7;
    }

    // 纵向
    addToRoad(x, y - 5, 0, weight);
    addToRoad(x, y - 4, 0, weight);
    addToRoad(x, y - 3, 0, weight);
    addToRoad(x, y - 2, 0, weight);
    addToRoad(x, y - 1, 0, weight);
    addToRoad(x, y, 0, weight);

    // 横向
    addToRoad(x - 5, y, 1, weight);
    addToRoad(x - 4, y, 1, weight);
    addToRoad(x - 3, y, 1, weight);
    addToRoad(x - 2, y, 1, weight);
    addToRoad(x - 1, y, 1, weight);
    addToRoad(x, y, 1, weight);

    // 右下对角
    addToRoad(x - 5, y - 5, 2, weight);
    addToRoad(x - 4, y - 4, 2, weight);
    addToRoad(x - 3, y - 3, 2, weight);
    addToRoad(x - 2, y - 2, 2, weight);
    addToRoad(x - 1, y - 1, 2, weight);
    addToRoad(x, y, 2, weight);

    // 左下对角
    addToRoad(x + 5, y - 5, 3, weight);
    addToRoad(x + 4, y - 4, 3, weight);
    addToRoad(x + 3, y - 3, 3, weight);
    addToRoad(x + 2, y - 2, 3, weight);
    addToRoad(x + 1, y - 1, 3, weight);
    addToRoad(x, y, 3, weight);
}


void Grid::addToRoad(int x, int y, int type, int weight) {
    if (x < 0 || x >= GRID_SIZE || y < 0 || y >= GRID_SIZE) return;
    if (type == 0 && y >= GRID_SIZE - 5) return;
    if (type == 1 && x >= GRID_SIZE - 5) return;
    if (type == 2 && (x >= GRID_SIZE - 5 || y >= GRID_SIZE - 5)) return;
    if (type == 3 && (x < 5 || y >= GRID_SIZE - 5)) return;

    road[y][x][type] += weight;
}


void Grid::undoStep(int x, int y) {
    if (data[y][x] == WHITE) {
        updateRoad(x, y, -7);
    } else if (data[y][x] == BLACK) {
        updateRoad(x, y, -1);
    }

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

    // road
    for (int type=0; type<4; type++) {
        std::cout << "COUNT " << type + 1 << ":" << std::endl;
        for (int y=0; y<GRID_SIZE; y++) {
            for (int x=0; x<GRID_SIZE; x++) {
                if (road[y][x][type] < 10) {
                    std::cout << " ";
                }
                std::cout << road[y][x][type] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
}


