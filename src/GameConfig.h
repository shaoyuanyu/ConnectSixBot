//
// 全局配置
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
 * 一步
 */
struct Step {
    int x;
    int y;

    long weight = 0;

    Step(int x, int y): x(x), y(y) {}

    bool operator<(const Step& other) const {
        return weight < other.weight;
    }
};

/**
 * 一个Move包含两个Step
 */
struct Move {
    int x0, y0;
    int x1, y1;

    Move(int x0, int y0, int x1, int y1): x0(x0), y0(y0), x1(x1), y1(y1) {}
    Move(Step step0, Step step1): x0(step0.x), y0(step0.y), x1(step1.x), y1(step1.y) {}
};


#endif //CONNECT_SIX_BOT_GAME_CONFIG_H
