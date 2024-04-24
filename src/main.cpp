//
// 六子棋
// ysy & lnz
//

#include <iostream>
#include "Grid.h"
#include "Bot.h"
#include "Evaluator.h"

int inputGrid(Grid& grid, Bot& bot) {
    int x0, y0, x1, y1;
    int turnId;
    Move move(-1, -1, -1, -1);

    std::cin >> turnId;

    // 假设我方执白
    bot.setColor(WHITE);

    for (int i=0; i<turnId; i++) {
        std::cin >> x0 >> y0 >> x1 >> y1;

        if (x0 == -1) {
            // 我方执黑
            bot.setColor(BLACK);
        }

        Evaluator evaluator(grid, bot.getColor());

        // 对手落子
        move = Move(x0, y0, x1, y1);
        grid.doMove(move, bot.getOppositeColor());

        // 我方落子
        if (i < turnId - 1) {
            std::cin >> x0 >> y0 >> x1 >> y1;

            move = Move(x0, y0, x1, y1);
            grid.doMove(move, bot.getColor());
        }
    }

    return turnId;
}

int main() {
    std::ios::sync_with_stdio(false);

    Grid grid;
    Bot bot;

    int turnId = inputGrid(grid, bot);

//    grid.output();

    Move result = bot.makeDecision(grid, turnId);

    std::cout << result.x0 << ' ' << result.y0 << ' ' << result.x1 << ' ' << result.y1 << std::endl;

    return 0;
}