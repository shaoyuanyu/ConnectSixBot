//
// ysy
// 2024/3/29
//

#ifndef CONNECT_SIX_BOT_GRID_H
#define CONNECT_SIX_BOT_GRID_H


#include <vector>
#include <queue>
#include <cmath>
#include "GameConfig.h"

struct Grid {
    Color data[GRID_SIZE][GRID_SIZE] = { BLANK };
    int weight[GRID_SIZE][GRID_SIZE] = { 0 };

    void doStep(int x, int y, Color color);
    inline void addWeight(int x, int y, int addition);

    std::vector<Step> getAvailable(int topK);

    void output();
};


/**
 * 用于候选位置的排序
 */
struct StepCandidate {
    Step step;
    int weight;

    StepCandidate(Step step, int weight): step(step), weight(weight) {}

    bool operator<(const StepCandidate& other) {
        if (weight == other.weight) {
            // 如果权重相等则倾向居中的位置
            return pow(step.x-7, 2) + pow(step.y-7, 2) > pow(other.step.x-7, 2) + pow(other.step.y-7, 2);
        }
        return weight < other.weight;
    }
};


#endif //CONNECT_SIX_BOT_GRID_H
