//
// Created by ysy on 24-4-1.
//

#ifndef CONNECT_SIX_BOT_GAME_TREE_H
#define CONNECT_SIX_BOT_GAME_TREE_H


#include <vector>
#include <float.h>
#include "GameConfig.h"
#include "Grid.h"

struct GameNode {
    float score = 0.0;
    Turn turn; // 一回合，两手落子
    bool isMaxNode; // 是否为Max节点
    std::vector<GameNode*> children;
    float alpha = FLT_MAX; // 上确界
    float beta = FLT_MIN; // 下确界

    GameNode(): turn(Turn(-1, -1, -1, -1)), isMaxNode(true) {}
    GameNode(Turn turn, bool isMaxNode, float alpha, float beta): turn(turn), isMaxNode(isMaxNode), alpha(alpha), beta(beta) {}

    void addChild(GameNode* child);
};


class GameTree {
private:
    GameNode* root = new GameNode();
    int depthLimit = 1; // 每次推理深度

    Color botColor;

//    float simulateStep(GameNode*& currentRoot, Grid currentGrid, std::vector<Turn> preTurns, Color currentColor, int turnCount);
    float simulateStep(GameNode*& currentRoot, const Grid& currentGrid, const std::vector<Turn>& preTurns, Color currentColor, int turnCount, int currentDepthLimit);
//    float evaluate(Grid grid, Step center);
    float evaluate(Grid grid, Step step);

public:
    explicit GameTree(Color botColor): botColor(botColor) {}

    Turn makeDecision(Grid grid);
};


#endif //CONNECT_SIX_BOT_GAME_TREE_H
