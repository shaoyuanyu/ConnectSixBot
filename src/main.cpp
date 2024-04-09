//
// ysy
// 2024/3/29
//

#include <iostream>
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
    std::ios::sync_with_stdio(false);

    Grid grid;
    Bot bot;

    int turnId = inputGrid(grid, bot);

    // 我方为黑开局
    if (turnId == 1 && bot.getColor() == BLACK) {
        std::cout << 7 << ' ' << 7 << ' ' << -1 << ' ' << -1 << std::endl;
        return 0;
    }

    std::cout << "make decision..." << std::endl;
    grid.output();
    Turn result = bot.makeDecision(grid);

    std::cout << result.x0 << ' ' << result.y0 << ' ' << result.x1 << ' ' << result.y1 << std::endl;

    return 0;
}