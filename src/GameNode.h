//
// Created by ysy on 24-4-9.
//

#ifndef CONNECTSIXBOT_GAMENODE_H
#define CONNECTSIXBOT_GAMENODE_H


#include <vector>
#include <cfloat>
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


#endif //CONNECTSIXBOT_GAMENODE_H
