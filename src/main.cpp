//
// 六子棋
//

#include <iostream>
#include <sys/time.h>
#include "Grid.h"
#include "Bot.h"

int inputGrid(Grid& grid, Bot& bot) {
    int x0, y0, x1, y1;
    int turnId;

    std::cin >> turnId;

    // 假设我方执白
    bot.setColor(WHITE);

    for (int i=0; i<turnId; i++) {
        std::cin >> x0 >> y0 >> x1 >> y1;

        if (x0 == -1) {
            // 我方执黑
            bot.setColor(BLACK);
        }

        // 对手落子
        grid.doStep(x0, y0, bot.getOppositeColor());
        grid.doStep(x1, y1, bot.getOppositeColor());

        // 我方落子
        if (i < turnId - 1) {
//            std::cout << "my step: ";
            std::cin >> x0 >> y0 >> x1 >> y1;

            grid.doStep(x0, y0, bot.getColor());
            grid.doStep(x1, y1, bot.getColor());
        }
    }

    return turnId;
}

int main() {
    struct timeval s_tv, e_tv;

    std::ios::sync_with_stdio(false);

    Grid grid;
    Bot bot;

    int turnId = inputGrid(grid, bot);

    gettimeofday(&s_tv, NULL);

    // 调试输出
    std::cout << "make decision..." << std::endl;
    grid.output();

    Turn result = bot.makeDecision(grid, turnId);

    gettimeofday(&e_tv, NULL);

    std::cout << result.x0 << ' ' << result.y0 << ' ' << result.x1 << ' ' << result.y1 << std::endl;

    std::cout << "started at: " << s_tv.tv_sec << ", " << s_tv.tv_usec << std::endl;
    std::cout << "ended at: " << e_tv.tv_sec << ", " << e_tv.tv_usec << std::endl;
    std::cout << "sec: " << e_tv.tv_sec - s_tv.tv_sec << std::endl;

    return 0;
}