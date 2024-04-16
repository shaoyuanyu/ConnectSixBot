//
// 基于博弈树的智能体
//

#ifndef CONNECT_SIX_BOT_BOT_H
#define CONNECT_SIX_BOT_BOT_H


#include <vector>
#include <queue>
#include "GameConfig.h"
#include "Grid.h"
#include "GameNode.h"
#include "Evaluator.h"

class Bot {
private:
    Color botColor;
    const int DEPTH_LIMIT = 3; // 每次推理深度
    const int TOP_K = 20;

    Evaluator evaluator;

    std::vector<GameNode*> firstTurnNodes;

    std::vector<Step> availableSteps;

    Turn makeOpening();
    Turn simulateStep(Grid& grid);
    void preSimulate(Grid& grid);
    float simulateStep(GameNode*& currentRoot, Grid& currentGrid, Color currentColor, int turnCount);

public:
    void setColor(Color c);
    Color getColor() const; // 获取我方颜色
    Color getOppositeColor() const; // 获取对手颜色
    Turn makeDecision(Grid& grid, const int& turnId); // 基于博弈树进行决策
};


#endif //CONNECT_SIX_BOT_BOT_H
