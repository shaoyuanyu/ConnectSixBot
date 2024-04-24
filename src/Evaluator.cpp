#include "Evaluator.h"

/**
 * Evaluator构造方法
 */
Evaluator::Evaluator(Grid& grid, Color botColor): grid(grid), botColor(botColor) {
    scoreOfMyRoad[0] = 0;
    scoreOfMyRoad[1] = 1;
    scoreOfMyRoad[2] = 20;
    scoreOfMyRoad[3] = 200;
    scoreOfMyRoad[4] = 100000;
    scoreOfMyRoad[5] = 10000000;
    scoreOfMyRoad[6] = 100000000000;

    scoreOfEnemyRoad[0] = 0;
    scoreOfEnemyRoad[1] = 1;
    scoreOfEnemyRoad[2] = 20;
    scoreOfEnemyRoad[3] = 200;
    scoreOfEnemyRoad[4] = 100000;
    scoreOfEnemyRoad[5] = 10000000;
    scoreOfEnemyRoad[6] = 100000000000;
}


/**
 * 全局评估
 */
long Evaluator::evaluate() {
    return calScore();
}


/**
 * 局部评估
 */
long Evaluator::evaluate(Move move) {
    return calScore(move);
}


/**
 * 预评估
 */
long Evaluator::preEvaluate(Step step) {
    return calScore(step);
}


/**
 * 计数
 * 黑棋加一分，白棋加7分
 */
inline int Evaluator::getCount(int x, int y) {
    return (grid.data[y][x] == WHITE) ? 7 : grid.data[y][x];
}


/**
 * 从黑1白7的count恢复自然计数。同时，与bot同色记为正，异色记为负。
 */
inline int Evaluator::revertCount(int count) const {
    if (count >= 7) {
        if (botColor == WHITE) {
            return count / 7;
        } else {
            return - count / 7;
        }
    } else {
        if (botColor == BLACK) {
            return count;
        } else {
            return -count;
        }
    }
}


/**
 * 根据count计算分数，全局
 */
long Evaluator::calScore() {
    long score = 0;
    std::vector<int> countOfMyRoad = std::vector<int>(10, 0);
    std::vector<int> countOfEnemyRoad = std::vector<int>(10, 0);

    // 统计
    for (int x=0; x<GRID_SIZE; x++) {
        for (int y=0; y<GRID_SIZE; y++) {
            for (int type=0; type<=3; type++) {
                int count = grid.count[y][x][type];

                if (count > 0) countOfMyRoad[count]++;
                else if (count < 0) countOfEnemyRoad[-count]++;
            }
        }
    }

    // 去重
    for (int i=1; i<=6; i++) {
        countOfMyRoad[i] /= i;
        countOfEnemyRoad[i] /= 1;
    }

    // 双四路 = 六路
    if (countOfMyRoad[4] >= 2) {
        countOfMyRoad[6] = countOfMyRoad[4] / 2;
        countOfMyRoad[4] %= 2;
    }
    if (countOfEnemyRoad[4] >= 2) {
        countOfEnemyRoad[6] = countOfEnemyRoad[4] / 2;
        countOfEnemyRoad[6] %= 2;
    }

    // 四路 + 五路 = 六路
    if (countOfMyRoad[4] > 0 && countOfMyRoad[5] > 0) {
        int pairs = (countOfMyRoad[4] <= countOfMyRoad[5]) ? countOfMyRoad[4] : countOfMyRoad[5];
        countOfMyRoad[4] -= pairs;
        countOfMyRoad[5] -= pairs;
        countOfMyRoad[6] += pairs;
    }
    if (countOfEnemyRoad[4] > 0 && countOfEnemyRoad[5] > 0) {
        int pairs = (countOfEnemyRoad[4] <= countOfEnemyRoad[5]) ? countOfEnemyRoad[4] : countOfEnemyRoad[5];
        countOfEnemyRoad[4] -= pairs;
        countOfEnemyRoad[5] -= pairs;
        countOfEnemyRoad[6] += pairs;
    }

    // 双五路 = 六路
    if (countOfMyRoad[5] >= 2) {
        countOfMyRoad[6] = countOfMyRoad[5] / 2;
        countOfMyRoad[5] %= 2;
    }
    if (countOfEnemyRoad[5] >= 2) {
        countOfEnemyRoad[6] = countOfEnemyRoad[5] / 2;
        countOfEnemyRoad[5] %= 2;
    }

    // 计分
    for (int i=1; i<=6; i++) {
        score += (countOfMyRoad[i] * scoreOfMyRoad[i] - countOfEnemyRoad[i] * scoreOfEnemyRoad[i]);
    }

    return score;
}


/**
 * 根据count计算分数，Move局部，用于判断是否在推理的中间步骤出现6子
 */
long Evaluator::calScore(Move move) {
    long score = 0;
    std::vector<int> countOfMyRoad = std::vector<int>(10, 0);
    std::vector<int> countOfEnemyRoad = std::vector<int>(10, 0);

    // 统计
    for (int type=0; type<=3; type++) {
        int count = grid.count[move.y0][move.x0][type];

        if (count > 0) countOfMyRoad[count]++;
        else if (count < 0) countOfEnemyRoad[-count]++;
    }
    for (int type=0; type<=3; type++) {
        int count = grid.count[move.y1][move.x1][type];

        if (count > 0) countOfMyRoad[count]++;
        else if (count < 0) countOfEnemyRoad[-count]++;
    }

    // 计分
    score += (countOfMyRoad[6] * scoreOfMyRoad[6] - countOfEnemyRoad[6] * scoreOfEnemyRoad[6]);

    return score;
}


/**
 * 根据count计算分数，Step局部，用于预评估
 */
long Evaluator::calScore(Step step) {
    long score = 0;
    std::vector<int> countOfMyRoad = std::vector<int>(10, 0);
    std::vector<int> countOfEnemyRoad = std::vector<int>(10, 0);

    // 统计
    for (int type=0; type<=3; type++) {
        int count = grid.count[step.y][step.x][type];

        if (count > 0) countOfMyRoad[count]++;
        else if (count < 0) countOfEnemyRoad[-count]++;
    }

    // 计分
    score += (countOfMyRoad[6] * scoreOfMyRoad[6] - countOfEnemyRoad[6] * scoreOfEnemyRoad[6]);

    return score;
}


/**
 * Move增量扫描
 */
void Evaluator::scan(Move move) {
    int maxCount, maxX, maxY;
    bool isRepeated;

    /**
     * 0纵向扫描 step0
     */
    maxCount = 0, maxX = -1, isRepeated = false;
    for (int x=move.x0, y=move.y0-5; y<GRID_SIZE-5 && y<=move.y0; y++) {
        if (y < 0) continue;

        int count = getCount(x, y) + getCount(x, y + 1) + getCount(x, y + 2) + getCount(x, y + 3) + getCount(x, y + 4) + getCount(x, y + 5);

        // 归零grid.count
        grid.count[y][x][0] = 0;

        // 黑白混杂不计入
        if (count > 6 && count % 7 != 0) continue;

        // 标记重复
        if (x == move.x1 && (y == move.y1 || y + 1 == move.y1 || y + 2 == move.y1 || y + 3 == move.y1 || y + 4 == move.y1 || y + 5 == move.y1)) isRepeated = true;

        // 更新max
        if (count >= maxCount) {
            maxCount = count;
            maxX = x;
            maxY = y;
        }
    }

    // 更新grid.count
    if (maxX != -1) {
        for (int shift=0; shift<6; shift++) {
            if (grid.data[maxY+shift][maxX] == BLANK) continue;
            grid.count[maxY+shift][maxX][0] = revertCount(maxCount);
        }
    }

    /**
     * 0纵向扫描 step1
     */
     if (!isRepeated) {
         maxCount = 0, maxX = -1;
         for (int x=move.x1, y=move.y1-5; y<GRID_SIZE-5 && y<=move.y1; y++) {
             if (y < 0) continue;
             if (x == move.x0 && (y == move.y0 || y + 1 == move.y0 || y + 2 == move.y0 || y + 3 == move.y0 || y + 4 == move.y0 || y + 5 == move.y0)) continue;

             int count = getCount(x, y) + getCount(x, y + 1) + getCount(x, y + 2) + getCount(x, y + 3) + getCount(x, y + 4) + getCount(x, y + 5);

             // 归零grid.count
             grid.count[y][x][0] = 0;

             // 黑白混杂不计入
             if (count > 6 && count % 7 != 0) continue;

             // 更新max
             if (count > maxCount) {
                 maxCount = count;
                 maxX = x;
                 maxY = y;
             }
         }

         // 更新grid.count
         if (maxX != -1) {
             for (int shift=0; shift<6; shift++) {
                 if (grid.data[maxY+shift][maxX] == BLANK) continue;
                 grid.count[maxY+shift][maxX][0] = revertCount(maxCount);
             }
         }
     }

    /**
     * 1横向扫描 step0
     */
    maxCount = 0, maxX = -1, isRepeated = false;
    for (int x=move.x0-5, y=move.y0; x<GRID_SIZE-5 && x<=move.x0; x++) {
        if (x < 0) continue;

        int count = getCount(x, y) + getCount(x + 1, y) + getCount(x + 2, y) + getCount(x + 3, y) + getCount(x + 4, y) + getCount(x + 5, y);

        // 归零grid.count
        grid.count[y][x][1] = 0;

        // 黑白混杂不计入
        if (count > 6 && count % 7 != 0) continue;

        // 标记重复
        if (y == move.y1 && (x == move.x1 || x + 1 == move.x1 || x + 2 == move.x1 || x + 3 == move.x1 || x + 4 == move.x1 || x + 5 == move.x1)) isRepeated = true;

        // 更新max
        if (count > maxCount) {
            maxCount = count;
            maxX = x;
            maxY = y;
        }
    }

    // 更新grid.count
    if (maxX != -1) {
        for (int shift=0; shift<6; shift++) {
            if (grid.data[maxY][maxX+shift] == 0) continue;
            grid.count[maxY][maxX+shift][1] = revertCount(maxCount);
        }
    }

    /**
     * 1横向扫描 step1
     */
     if (!isRepeated) {
         maxCount = 0, maxX = -1;
         for (int x=move.x1-5, y=move.y1; x<GRID_SIZE-5 && x<=move.x1; x++) {
             if (x < 0) continue;
             if (y == move.y0 && (x == move.x0 || x + 1 == move.x0 || x + 2 == move.x0 || x + 3 == move.x0 || x + 4 == move.x0 || x + 5 == move.x0)) continue;

             int count = getCount(x, y) + getCount(x + 1, y) + getCount(x + 2, y) + getCount(x + 3, y) + getCount(x + 4, y) + getCount(x + 5, y);

             // 归零grid.count
             grid.count[y][x][1] = 0;

             // 黑白混杂不计入
             if (count > 6 && count % 7 != 0) continue;

             // 更新max
             if (count > maxCount) {
                 maxCount = count;
                 maxX = x;
                 maxY = y;
             }
         }

         // 更新grid.count
         if (maxX != -1) {
             for (int shift=0; shift<6; shift++) {
                 if (grid.data[maxY][maxX+shift] == 0) continue;
                 grid.count[maxY][maxX+shift][1] = revertCount(maxCount);
             }
         }
     }

    /**
     * 2右下对角扫描 step0
     */
    maxCount = 0, maxX = -1, isRepeated = false;
    for (int x=move.x0-5, y=move.y0-5; (x<GRID_SIZE-5 && x<=move.x0) && (y<GRID_SIZE-5 && y<=move.y0); x++, y++) {
        if (x < 0 || y < 0) continue;

        int count = getCount(x, y) + getCount(x + 1, y + 1) + getCount(x + 2, y + 2) + getCount(x + 3, y + 3) + getCount(x + 4, y + 4) + getCount(x + 5, y + 5);

        // 归零grid.count
        grid.count[y][x][2] = 0;

        // 黑白混杂不计入
        if (count > 6 && count % 7 != 0) continue;

        // 标记重复
        if ((x == move.x1 && y == move.y1) || (x + 1 == move.x1 && y + 1 == move.y1) || (x + 2 == move.x1 && y + 2 == move.y1) || (x + 3 == move.x1 && y + 3 == move.y1) || (x + 4 == move.x1 && y + 4 == move.y1) || (x + 5 == move.x1 && y + 5 == move.y1)) isRepeated = true;

        // 更新max
        if (count > maxCount) {
            maxCount = count;
            maxX = x;
            maxY = y;
        }
    }

    // 更新grid.count
    if (maxX != -1) {
        for (int shift=0; shift<6; shift++) {
            if (grid.data[maxY+shift][maxX+shift] == 0) continue;
            grid.count[maxY+shift][maxX+shift][2] = revertCount(maxCount);
        }
    }

    /**
     * 2右下对角扫描 step1
     */
    if (!isRepeated) {
        maxCount = 0, maxX = -1;
        for (int x=move.x1-5, y=move.y1-5; (x<GRID_SIZE-5 && x<=move.x1) && (y<GRID_SIZE-5 && y<=move.y1); x++, y++) {
            if (x < 0 || y < 0) continue;
            if ((x == move.x0 && y == move.y0) || (x + 1 == move.x0 && y + 1 == move.y0) || (x + 2 == move.x0 && y + 2 == move.y0) || (x + 3 == move.x0 && y + 3 == move.y0) || (x + 4 == move.x0 && y + 4 == move.y0) || (x + 5 == move.x0 && y + 5 == move.y0)) continue;

            int count = getCount(x, y) + getCount(x + 1, y + 1) + getCount(x + 2, y + 2) + getCount(x + 3, y + 3) + getCount(x + 4, y + 4) + getCount(x + 5, y + 5);

            // 归零grid.count
            grid.count[y][x][2] = 0;

            // 黑白混杂不计入
            if (count > 6 && count % 7 != 0) continue;

            // 更新max
            if (count > maxCount) {
                maxCount = count;
                maxX = x;
                maxY = y;
            }
        }

        // 更新grid.count
        if (maxX != -1) {
            for (int shift=0; shift<6; shift++) {
                if (grid.data[maxY+shift][maxX+shift] == 0) continue;
                grid.count[maxY+shift][maxX+shift][2] = revertCount(maxCount);
            }
        }
    }

    /**
     * 3左下对角扫描 step0
     */
    maxCount = 0, maxX = -1, isRepeated = false;
    for (int x=move.x0+5, y=move.y0-5; (x>=5 && x>=move.x0) && (y<GRID_SIZE-5 && y<=move.y0); x--, y++) {
        if (x >= GRID_SIZE || y < 0) continue;

        int count = getCount(x, y) + getCount(x - 1, y + 1) + getCount(x - 2, y + 2) + getCount(x - 3, y + 3) + getCount(x - 4, y + 4) + getCount(x - 5, y + 5);

        // 归零grid.count
        grid.count[y][x][3] = 0;

        // 黑白混杂不计入
        if (count > 6 && count % 7 != 0) continue;

        // 标记重复
        if ((x == move.x1 && y == move.y1) || (x - 1 == move.x1 && y + 1 == move.y1) || (x - 2 == move.x1 && y + 2 == move.y1) || (x - 3 == move.x1 && y + 3 == move.y1) || (x - 4 == move.x1 && y + 4 == move.y1) || (x - 5 == move.x1 && y + 5 == move.y1)) isRepeated = true;

        // 更新max
        if (count > maxCount) {
            maxCount = count;
            maxX = x;
            maxY = y;
        }
    }

    // 更新grid.count
    if (maxX != -1) {
        for (int shift=0; shift<6; shift++) {
            if (grid.data[maxY+shift][maxX-shift] == 0) continue;
            grid.count[maxY+shift][maxX-shift][3] = revertCount(maxCount);
        }
    }

    /**
     * 3左下对角扫描 step1
     */
    if (!isRepeated) {
        maxCount = 0, maxX = -1;
        for (int x=move.x1+5, y=move.y1-5; (x>=5 && x>=move.x1) && (y<GRID_SIZE-5 && y<=move.y1); x--, y++) {
            if (x >= GRID_SIZE || y < 0) continue;
            if ((x == move.x0 && y == move.y0) || (x - 1 == move.x0 && y + 1 == move.y0) || (x - 2 == move.x0 && y + 2 == move.y0) || (x - 3 == move.x0 && y + 3 == move.y0) || (x - 4 == move.x0 && y + 4 == move.y0) || (x - 5 == move.x0 && y + 5 == move.y0)) continue;

            int count = getCount(x, y) + getCount(x - 1, y + 1) + getCount(x - 2, y + 2) + getCount(x - 3, y + 3) + getCount(x - 4, y + 4) + getCount(x - 5, y + 5);

            // 归零grid.count
            grid.count[y][x][3] = 0;

            // 黑白混杂不计入
            if (count > 6 && count % 7 != 0) continue;

            // 更新max
            if (count > maxCount) {
                maxCount = count;
                maxX = x;
                maxY = y;
            }
        }

        // 更新grid.count
        if (maxX != -1) {
            for (int shift=0; shift<6; shift++) {
                if (grid.data[maxY+shift][maxX-shift] == 0) continue;
                grid.count[maxY+shift][maxX-shift][3] = revertCount(maxCount);
            }
        }
    }
}


/**
 * Step增量扫描
 */
void Evaluator::scan(Step step) {
    int maxCount, maxX, maxY;

    /**
     * 0纵向扫描
     */
    maxCount = 0, maxX = -1;
    for (int x=step.x, y=step.y-5; y<GRID_SIZE-5 && y<=step.y; y++) {
        if (y < 0) continue;

        int count = getCount(x, y) + getCount(x, y + 1) + getCount(x, y + 2) + getCount(x, y + 3) + getCount(x, y + 4) + getCount(x, y + 5);

        // 归零grid.count
        grid.count[y][x][0] = 0;

        // 黑白混杂不计入
        if (count > 6 && count % 7 != 0) continue;

        // 更新max
        if (count >= maxCount) {
            maxCount = count;
            maxX = x;
            maxY = y;
        }
    }


    // 更新grid.count
    if (maxX != -1) {
        for (int shift=0; shift<6; shift++) {
            if (grid.data[maxY+shift][maxX] == BLANK) continue;
            grid.count[maxY+shift][maxX][0] = revertCount(maxCount);
        }
    }

    /**
     * 1横向扫描
     */
    maxCount = 0, maxX = -1;
    for (int x=step.x-5, y=step.y; x<GRID_SIZE-5 && x<=step.x; x++) {
        if (x < 0) continue;

        int count = getCount(x, y) + getCount(x + 1, y) + getCount(x + 2, y) + getCount(x + 3, y) + getCount(x + 4, y) + getCount(x + 5, y);

        // 归零grid.count
        grid.count[y][x][1] = 0;

        // 黑白混杂不计入
        if (count > 6 && count % 7 != 0) continue;

        // 更新max
        if (count > maxCount) {
            maxCount = count;
            maxX = x;
            maxY = y;
        }
    }

    // 更新grid.count
    if (maxX != -1) {
        for (int shift=0; shift<6; shift++) {
            if (grid.data[maxY][maxX+shift] == 0) continue;
            grid.count[maxY][maxX+shift][1] = revertCount(maxCount);
        }
    }

    /**
     * 2右下对角扫描
     */
    maxCount = 0, maxX = -1;
    for (int x=step.x-5, y=step.y-5; (x<GRID_SIZE-5 && x<=step.x) && (y<GRID_SIZE-5 && y<=step.y); x++, y++) {
        if (x < 0 || y < 0) continue;

        int count = getCount(x, y) + getCount(x + 1, y + 1) + getCount(x + 2, y + 2) + getCount(x + 3, y + 3) + getCount(x + 4, y + 4) + getCount(x + 5, y + 5);

        // 归零grid.count
        grid.count[y][x][2] = 0;

        // 黑白混杂不计入
        if (count > 6 && count % 7 != 0) continue;

        // 更新max
        if (count > maxCount) {
            maxCount = count;
            maxX = x;
            maxY = y;
        }
    }

    // 更新grid.count
    if (maxX != -1) {
        for (int shift=0; shift<6; shift++) {
            if (grid.data[maxY+shift][maxX+shift] == 0) continue;
            grid.count[maxY+shift][maxX+shift][2] = revertCount(maxCount);
        }
    }

    /**
     * 3左下对角扫描
     */
    maxCount = 0, maxX = -1;
    for (int x=step.x+5, y=step.y-5; (x>=5 && x>=step.x) && (y<GRID_SIZE-5 && y<=step.y); x--, y++) {
        if (x >= GRID_SIZE || y < 0) continue;

        int count = getCount(x, y) + getCount(x - 1, y + 1) + getCount(x - 2, y + 2) + getCount(x - 3, y + 3) + getCount(x - 4, y + 4) + getCount(x - 5, y + 5);

        // 归零grid.count
        grid.count[y][x][3] = 0;

        // 黑白混杂不计入
        if (count > 6 && count % 7 != 0) continue;

        // 更新max
        if (count > maxCount) {
            maxCount = count;
            maxX = x;
            maxY = y;
        }
    }

    // 更新grid.count
    if (maxX != -1) {
        for (int shift=0; shift<6; shift++) {
            if (grid.data[maxY+shift][maxX-shift] == 0) continue;
            grid.count[maxY+shift][maxX-shift][3] = revertCount(maxCount);
        }
    }
}

