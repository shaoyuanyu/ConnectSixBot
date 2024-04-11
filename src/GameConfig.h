//
// ysy
// 2024/3/29
//

#ifndef CONNECT_SIX_BOT_GAME_CONFIG_H
#define CONNECT_SIX_BOT_GAME_CONFIG_H

/**
 * 棋盘
 */
#define GRID_SIZE 15

/**
 * 棋子颜色
 */
#define WHITE (-1)
#define BLANK 0
#define BLACK 1
typedef int Color;

/**
 * 一手落子
 */
struct Step {
    int x;
    int y;

    Step(int x, int y): x(x), y(y) {}
};

/**
 * 一回合包括两手落子
 */
struct Turn {
    int x0, y0;
    int x1, y1;

    Turn(int x0, int y0, int x1, int y1): x0(x0), y0(y0), x1(x1), y1(y1) {}
    Turn(Step step0, Step step1): x0(step0.x), y0(step0.y), x1(step1.x), y1(step1.y) {}
};
// TODO: Turn能否用两个Step组合


#endif //CONNECT_SIX_BOT_GAME_CONFIG_H
