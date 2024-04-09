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
    GameNode* root = new GameNode();
    int basicDepthLimit = 1; // 每次推理深度

    Turn makeOpening();
    float simulateStep(GameNode*& currentRoot, const Grid& currentGrid, const std::vector<Turn>& preTurns, Color currentColor, int turnCount, int currentDepthLimit);
    float evaluate(Grid grid, Step step);
    inline bool count(const Grid& grid, const int& x, const int& y, const Color& currentColor, float& count);

public:
    void setColor(Color c);
    Color getColor(); // 获取我方颜色
    Color getOppositeColor(); // 获取对手颜色
    Turn makeDecision(Grid& grid, int turnId); // 基于博弈树进行决策
};


#endif //CONNECT_SIX_BOT_BOT_H
