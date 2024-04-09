//
// ysy
// 2024/3/28, 29
//
#include "Bot.h"
#include "GameTree.h"

void Bot::setColor(Color c) {
    botColor = c;
}

Color Bot::getColor() {
    return botColor;
}

Color Bot::getOppositeColor() {
    return -botColor;
}

Turn Bot::makeDecision(Grid& grid) {
    // 基于博弈树进行决策
    GameTree gameTree(botColor);
    return gameTree.makeDecision(grid);
}