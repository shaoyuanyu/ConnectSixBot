//
// ysy
// 2024/3/29
//

#ifndef CONNECT_SIX_BOT_BOT_H
#define CONNECT_SIX_BOT_BOT_H


#include "GameConfig.h"
#include "Grid.h"

class Bot {
private:
    Color botColor;

    Turn makeOpening(Grid& grid); // 基于开局库进行开局决策

public:
    void setColor(Color c);
    Color getColor(); // 获取我方颜色
    Color getOppositeColor(); // 获取对手颜色
    Turn makeDecision(Grid& grid); // 基于博弈树进行决策
};


#endif //CONNECT_SIX_BOT_BOT_H
