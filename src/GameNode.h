//
// 博弈树节点
//

#ifndef CONNECT_SIX_BOT_GAME_NODE_H
#define CONNECT_SIX_BOT_GAME_NODE_H


#include <vector>
#include <climits>
#include "GameConfig.h"
#include "Grid.h"

struct GameNode {
    long score = 0;
    Move move; // 一回合，两手落子
    bool isMaxNode; // 是否为Max节点
    long alpha = LONG_MAX; // 上确界
    long beta = -LONG_MAX; // 下确界

    GameNode(Move move, bool isMaxNode): move(move), isMaxNode(isMaxNode) {}
    GameNode(Move move, bool isMaxNode, long alpha, long beta): move(move), isMaxNode(isMaxNode), alpha(alpha), beta(beta) {}
};


#endif //CONNECT_SIX_BOT_GAME_NODE_H
