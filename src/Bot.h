//
// ysy
// 2024/3/29
//

#ifndef CONNECT_SIX_BOT_BOT_H
#define CONNECT_SIX_BOT_BOT_H


#include "BotColor.h"
#include "Result.h"
#include "Grid.h"

class Bot {
private:
    BotColor currentColor;

    Result makeOpening(Grid& grid); // 基于开局库进行开局决策

public:
    void setColor(BotColor c);
    BotColor getColor(); // 获取我方颜色
    BotColor getOppositeColor(); // 获取对手颜色
    Result makeDecision(Grid& grid); // 基于博弈树进行决策
};


#endif //CONNECT_SIX_BOT_BOT_H
