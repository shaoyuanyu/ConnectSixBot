//
// ysy
// 2024/3/29
//

#include "Grid.h"
#include <iostream>

void Grid::doStep(int x, int y, Color color) {
    if (x == -1) return;

    if (data[x][y] == BLANK) {
        data[x][y] = color;

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


/**
 * 利用priority_queue从棋盘中选出前topK个（如果存在）的点位
 */
std::vector<Step> Grid::getAvailable(const int topK) {
    std::priority_queue<StepCandidate, std::vector<StepCandidate>, std::less<>> stepCandidates;
    std::vector<Step> availableSteps;

    for (int x=0; x<GRID_SIZE; x++) {
        for (int y=0; y<GRID_SIZE; y++) {
            if (data[x][y] != BLANK) continue;
            stepCandidates.push(
                    StepCandidate(Step(x, y), weight[x][y])
            );
        }
    }

    for (int i=0; i<topK && !stepCandidates.empty(); i++) {
        availableSteps.push_back(stepCandidates.top().step);
        stepCandidates.pop();
    }

    return availableSteps;
}


void Grid::output() {
    for (int x=0; x<GRID_SIZE; x++) {
        for (int y=0; y<GRID_SIZE; y++) {
            std::cout << data[x][y] << " ";
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