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

        for (int i=1; i<=5; i++) {
            int w = 1;
            addWeight(x-i, y, w);
            addWeight(x+i, y, w);
            addWeight(x, y-i, w);
            addWeight(x, y+i, w);
            addWeight(x-i, y-i, w);
            addWeight(x-i, y+i, w);
            addWeight(x+i, y-i, w);
            addWeight(x+i, y+i, w);
        }
    }
}

inline void Grid::addWeight(int x, int y, int addition) {
    if (x<0 || x>=GRID_SIZE || y<0 || y>=GRID_SIZE) return;

    weight[x][y] += addition;
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
            if (weight[x][y] == 0) continue;

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