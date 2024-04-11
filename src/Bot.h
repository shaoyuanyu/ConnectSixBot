//
// ysy
// 2024/3/29
//

#ifndef CONNECT_SIX_BOT_BOT_H
#define CONNECT_SIX_BOT_BOT_H


#include <vector>
#include "GameConfig.h"
#include "Grid.h"
#include "GameNode.h"

class Bot {
private:
    Color botColor;
    int basicDepthLimit = 1; // 每次推理深度
    int topK = 20;

    Turn maxTurn = Turn(-1, -1, -1, -1);

    std::vector<GameNode*> firstTurnNodes;

    Turn makeOpening();
    inline int getDepthByWeight(int weight0, int weight1);
    float simulateStep(GameNode*& currentRoot, Grid& currentGrid, const std::vector<Turn>& preTurns, Color currentColor, int turnCount, int currentDepthLimit);
    float evaluate(Grid grid, Step step);
    inline bool count(const Grid& grid, const int& x, const int& y, const Color& currentColor, float& count);

public:
    void setColor(Color c);
    Color getColor() const; // 获取我方颜色
    Color getOppositeColor() const; // 获取对手颜色
    Turn makeDecision(Grid& grid, const int& turnId); // 基于博弈树进行决策
};


#endif //CONNECT_SIX_BOT_BOT_H
