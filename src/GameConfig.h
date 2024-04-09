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
};

/**
 * 一回合包括两手落子
 */
struct Turn {
    int x0, y0;
    int x1, y1;
};


#endif //CONNECT_SIX_BOT_GAME_CONFIG_H
