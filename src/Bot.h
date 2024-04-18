//
// 基于博弈树的智能体
//

#ifndef CONNECT_SIX_BOT_BOT_H
#define CONNECT_SIX_BOT_BOT_H


#include <vector>
#include <queue>
#include <cmath>
#include "GameConfig.h"
#include "Grid.h"
#include "GameNode.h"
#include "Evaluator.h"

class Bot {
private:
    Color botColor;
    const int DEPTH_LIMIT = 3; // 每次推理深度
    const int TOP_K = 20;

    std::vector<GameNode*> firstMoveNodes;

    std::vector<Step> availableSteps;

    Move makeOpening();
    Move simulateStep(Grid& grid);
    void preSimulate(Grid& grid);
    long simulateStep(GameNode*& currentNode, Grid& currentGrid, const Color currentColor, int moveCount);

public:
    void setColor(Color c);
    Color getColor() const; // 获取我方颜色
    Color getOppositeColor() const; // 获取对手颜色
    Move makeDecision(Grid& grid, const int& moveId); // 基于博弈树进行决策
};


#endif //CONNECT_SIX_BOT_BOT_H
