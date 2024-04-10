//
// Created by ysy on 24-4-9.
//

#ifndef CONNECT_SIX_BOT_GAME_NODE_H
#define CONNECT_SIX_BOT_GAME_NODE_H


#include <vector>
#include <cfloat>
#include "GameConfig.h"
#include "Grid.h"

struct GameNode {
    float score = 0.0;
    Turn turn; // 一回合，两手落子
    bool isMaxNode; // 是否为Max节点
    float alpha = FLT_MAX; // 上确界
    float beta = -FLT_MAX; // 下确界

    GameNode(): turn(Turn(-1, -1, -1, -1)), isMaxNode(true) {}
    GameNode(Turn turn, bool isMaxNode): turn(turn), isMaxNode(isMaxNode) {}
    GameNode(Turn turn, bool isMaxNode, float alpha, float beta): turn(turn), isMaxNode(isMaxNode), alpha(alpha), beta(beta) {}
};


#endif //CONNECT_SIX_BOT_GAME_NODE_H
