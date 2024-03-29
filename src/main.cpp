//
// ysy
// 2024/3/29
//

#include <iostream>
#include "Grid.h"
#include "Bot.h"

void inputGrid(Grid& grid, Bot& bot) {
    int x0, y0, x1, y1;
    int turnId;

    std::cin >> turnId;

    // 假设我方执白
    bot.setColor(white);

    for (int i=0; i<turnId; i++) {
        std::cin >> x0 >> y0 >> x1 >> y1;

        if (x0 == -1) {
            // 我方执黑
            bot.setColor(black);
        }

        // 对手落子
        grid.doStep(x0, y0, bot.getOppositeColor());
        grid.doStep(x1, y1, bot.getOppositeColor());

        // 我方落子
        if (i < turnId - 1) {
            std::cin >> x0 >> y0 >> x1 >> y1;

            grid.doStep(x0, y0, bot.getColor());
            grid.doStep(x1, y1, bot.getColor());
        }
    }
}

int main() {
    Grid grid;
    Bot bot;

    inputGrid(grid, bot);

    Result result = bot.makeDecision(grid);

    std::cout << result.x0 << ' ' << result.y0 << ' ' << result.x1 << ' ' << result.y1 << std::endl;

    return 0;
}