//
// ysy
// 2024/3/28, 29
//
#include "Bot.h"

void Bot::setColor(BotColor c) {
    currentColor = c;
}

BotColor Bot::getColor() {
    return currentColor;
}

BotColor Bot::getOppositeColor() {
    return -currentColor;
}

Result Bot::makeDecision(Grid &grid) {
    // 基于博弈树进行决策
    return Result(0, 0, 0, 0);
}