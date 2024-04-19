#include "Evaluator.h"

/**
 * Evaluator构造方法
 */
Evaluator::Evaluator(Grid grid, Color currentColor): grid(grid), currentColor(currentColor) {
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
 * 所下棋子的价值：下棋后局面分数 - 下棋前局面分数
 */
long Evaluator::evaluate() {
    return calScore();
}


/**
 *
 */
long Evaluator::evaluate(Move move) {
    return calScore(move);
}


/**
 * 计数
 * 黑棋加一分，白棋加7分
 */
inline int Evaluator::getCount(int x, int y) {
    return (grid.data[y][x] == WHITE) ? 7 : grid.data[y][x];
}


/**
 * 更新不同路类型的数量
 */
inline void Evaluator::updateRoadTypeNum(int count, std::vector<int>& countOfMyRoad, std::vector<int>& countOfEnemyRoad) const {
    // 没棋
    if (count == 0) return;
    // 黑白混杂
    if (count > 6 && count % 7 != 0) return;

    if (count < 7) {
        if (currentColor == BLACK) {
            countOfMyRoad[count]++;
        } else {
            countOfEnemyRoad[count]++;
        }
    } else {
        if (currentColor == BLACK) {
            countOfEnemyRoad[count/7]++;
        } else {
            countOfMyRoad[count/7]++;
        }
    }
}


/**
 * 计算此时局面的总分数(全局扫描)
 */
long Evaluator::calScore() {
    long score = 0;
    std::vector<int> countOfMyRoad = std::vector<int>(10, 0);
    std::vector<int> countOfEnemyRoad = std::vector<int>(10, 0);

    // 纵向扫描
    for (int x=0; x<GRID_SIZE; x++) {
        for (int y=0; y<GRID_SIZE-5; y++) {
            int count = getCount(x, y) + getCount(x, y + 1) +  getCount(x, y + 2) + getCount(x, y + 3) + getCount(x, y + 4) + getCount(x, y + 5);

            if (count > 6 && count % 7 != 0) continue; // 黑白混杂不计入

            updateRoadTypeNum(count, countOfMyRoad, countOfEnemyRoad);
        }
    }
    // 横向扫描
    for (int x=0; x<GRID_SIZE-5; x++) {
        for (int y=0; y<GRID_SIZE; y++) {
            int count = getCount(x, y) + getCount(x + 1, y) + getCount(x + 2, y) + getCount(x + 3, y) + getCount(x + 4, y) + getCount(x + 5, y);

            if (count > 6 && count % 7 != 0) continue; // 黑白混杂不计入

            updateRoadTypeNum(count, countOfMyRoad, countOfEnemyRoad);
        }
    }
    // 右下对角扫描
    for (int x=0; x<GRID_SIZE-5; x++) {
        for (int y=0; y<GRID_SIZE-5; y++) {
            int count = getCount(x, y) + getCount(x + 1, y + 1) + getCount(x + 2, y + 2) + getCount(x + 3, y + 3) + getCount(x + 4, y + 4) + getCount(x + 5, y + 5);

            if (count > 6 && count % 7 != 0) continue; // 黑白混杂不计入

            updateRoadTypeNum(count, countOfMyRoad, countOfEnemyRoad);
        }
    }
    // 左下对角扫描
    for (int x=GRID_SIZE-1; x>=5; x--) {
        for (int y=0; y<GRID_SIZE-5; y++) {
            int count = getCount(x, y) + getCount(x - 1, y + 1) + getCount(x - 2, y + 2) + getCount(x - 3, y + 3) + getCount(x - 4, y + 4) + getCount(x - 5, y + 5);

            if (count > 6 && count % 7 != 0) continue; // 黑白混杂不计入

            updateRoadTypeNum(count, countOfMyRoad, countOfEnemyRoad);
        }
    }

    //
    for (int i=1; i<=3; i++) {
        score += (countOfMyRoad[i] * scoreOfMyRoad[i] - countOfEnemyRoad[i] * scoreOfEnemyRoad[i]);
    }
    for (int i=4; i<=5; i++) {
        if (countOfMyRoad[i] >= 2 && countOfEnemyRoad[i] >= 2) {
            score += (countOfMyRoad[i] * scoreOfMyRoad[i+1] - countOfEnemyRoad[i] * scoreOfEnemyRoad[i+1]);
        } else if (countOfMyRoad[i] >= 2) {
            score += (countOfMyRoad[i] * scoreOfMyRoad[i+1] - countOfEnemyRoad[i] * scoreOfEnemyRoad[i]);
        } else if (countOfEnemyRoad[i] >= 2) {
            score += (countOfMyRoad[i] * scoreOfMyRoad[i] - countOfEnemyRoad[i] * scoreOfEnemyRoad[i+1]);
        } else {
            score += (countOfMyRoad[i] * scoreOfMyRoad[i] - countOfEnemyRoad[i] * scoreOfEnemyRoad[i]);
        }
    }
    score += (countOfMyRoad[6] * scoreOfMyRoad[6] - countOfEnemyRoad[6] * scoreOfEnemyRoad[6]);
    
    return score;
}


/**
 *
 */
long Evaluator::calScore(Move move) {
    long score = 0;
    std::vector<int> countOfMyRoad = std::vector<int>(10, 0);
    std::vector<int> countOfEnemyRoad = std::vector<int>(10, 0);

    int maxCount;
    // 纵向扫描 step0
    maxCount = 0;
    for (int x=move.x0, y=move.y0-5; y<GRID_SIZE-5 && y<=move.y0; y++) {
        if (y < 0) continue;

        int count = getCount(x, y) + getCount(x, y + 1) + getCount(x, y + 2) + getCount(x, y + 3) + getCount(x, y + 4) + getCount(x, y + 5);

        if (count > 6 && count % 7 != 0) continue; // 黑白混杂不计入
        if (count > maxCount) maxCount = count; // 更新max
    }
    updateRoadTypeNum(maxCount, countOfMyRoad, countOfEnemyRoad);
    // 纵向扫描 step1
    maxCount = 0;
    for (int x=move.x1, y=move.y1-5; y<GRID_SIZE-5 && y<=move.y1; y++) {
        if (y < 0) continue;
        if (x == move.x0 && (y == move.y0 || y + 1 == move.y0 || y + 2 == move.y0 || y + 3 == move.y0 || y + 4 == move.y0 || y + 5 == move.y0)) continue;

        int count = getCount(x, y) + getCount(x, y + 1) + getCount(x, y + 2) + getCount(x, y + 3) + getCount(x, y + 4) + getCount(x, y + 5);

        if (count > 6 && count % 7 != 0) continue; // 黑白混杂不计入
        if (count > maxCount) maxCount = count; // 更新max
    }
    updateRoadTypeNum(maxCount, countOfMyRoad, countOfEnemyRoad);
    // 横向扫描 step0
    maxCount = 0;
    for (int x=move.x0-5, y=move.y0; x<GRID_SIZE-5 && x<=move.x0; x++) {
        if (x < 0) continue;

        int count = getCount(x, y) + getCount(x + 1, y) + getCount(x + 2, y) + getCount(x + 3, y) + getCount(x + 4, y) + getCount(x + 5, y);

        if (count > 6 && count % 7 != 0) continue; // 黑白混杂不计入
        if (count > maxCount) maxCount = count; // 更新max
    }
    updateRoadTypeNum(maxCount, countOfMyRoad, countOfEnemyRoad);
    // 横向扫描 step1
    maxCount = 0;
    for (int x=move.x1-5, y=move.y1; x<GRID_SIZE-5 && x<=move.x1; x++) {
        if (x < 0) continue;
        if (y == move.y0 && (x == move.x0 || x + 1 == move.x0 || x + 2 == move.x0 || x + 3 == move.x0 || x + 4 == move.x0 || x + 5 == move.x0)) continue;

        int count = getCount(x, y) + getCount(x + 1, y) + getCount(x + 2, y) + getCount(x + 3, y) + getCount(x + 4, y) + getCount(x + 5, y);

        if (count > 6 && count % 7 != 0) continue; // 黑白混杂不计入
        if (count > maxCount) maxCount = count; // 更新max
    }
    updateRoadTypeNum(maxCount, countOfMyRoad, countOfEnemyRoad);
    // 右下对角扫描 step0
    maxCount = 0;
    for (int x=move.x0-5, y=move.y0-5; (x<GRID_SIZE-5 && x<=move.x0) && (y<GRID_SIZE-5 && y<=move.y0); x++, y++) {
        if (x < 0 || y < 0) continue;

        int count = getCount(x, y) + getCount(x + 1, y + 1) + getCount(x + 2, y + 2) + getCount(x + 3, y + 3) + getCount(x + 4, y + 4) + getCount(x + 5, y + 5);

        if (count > 6 && count % 7 != 0) continue; // 黑白混杂不计入
        if (count > maxCount) maxCount = count; // 更新max
    }
    updateRoadTypeNum(maxCount, countOfMyRoad, countOfEnemyRoad);
    // 右下对角扫描 step1
    maxCount = 0;
    for (int x=move.x1-5, y=move.y1-5; (x<GRID_SIZE-5 && x<=move.x1) && (y<GRID_SIZE-5 && y<=move.y1); x++, y++) {
        if (x < 0 || y < 0) continue;
        if ((x == move.x0 && y == move.y0) || (x + 1 == move.x0 && y + 1 == move.y0) || (x + 2 == move.x0 && y + 2 == move.y0) || (x + 3 == move.x0 && y + 3 == move.y0) || (x + 4 == move.x0 && y + 4 == move.y0) || (x + 5 == move.x0 && y + 5 == move.y0)) continue;

        int count = getCount(x, y) + getCount(x + 1, y + 1) + getCount(x + 2, y + 2) + getCount(x + 3, y + 3) + getCount(x + 4, y + 4) + getCount(x + 5, y + 5);

        if (count > 6 && count % 7 != 0) continue; // 黑白混杂不计入
        if (count > maxCount) maxCount = count; // 更新max
    }
    updateRoadTypeNum(maxCount, countOfMyRoad, countOfEnemyRoad);
    // 左下对角扫描 step0
    maxCount = 0;
    for (int x=move.x0+5, y=move.y0-5; (x>=5 && x>=move.x0) && (y<GRID_SIZE-5 && y<=move.y0); x--, y++) {
        if (x >= GRID_SIZE || y < 0) continue;

        int count = getCount(x, y) + getCount(x - 1, y + 1) + getCount(x - 2, y + 2) + getCount(x - 3, y + 3) + getCount(x - 4, y + 4) + getCount(x - 5, y + 5);

        if (count > 6 && count % 7 != 0) continue; // 黑白混杂不计入
        if (count > maxCount) maxCount = count; // 更新max
    }
    updateRoadTypeNum(maxCount, countOfMyRoad, countOfEnemyRoad);
    // 左下对角扫描 step1
    maxCount = 0;
    for (int x=move.x1+5, y=move.y1-5; (x>=5 && x>=move.x1) && (y<GRID_SIZE-5 && y<=move.y1); x--, y++) {
        if (x >= GRID_SIZE || y < 0) continue;
        if ((x == move.x0 && y == move.y0) || (x - 1 == move.x0 && y + 1 == move.y0) || (x - 2 == move.x0 && y + 2 == move.y0) || (x - 3 == move.x0 && y + 3 == move.y0) || (x - 4 == move.x0 && y + 4 == move.y0) || (x - 5 == move.x0 && y + 5 == move.y0)) continue;

        int count = getCount(x, y) + getCount(x - 1, y + 1) + getCount(x - 2, y + 2) + getCount(x - 3, y + 3) + getCount(x - 4, y + 4) + getCount(x - 5, y + 5);

        if (count > 6 && count % 7 != 0) continue; // 黑白混杂不计入
        if (count > maxCount) maxCount = count; // 更新max
    }
    updateRoadTypeNum(maxCount, countOfMyRoad, countOfEnemyRoad);

    //
    for (int i=1; i<=3; i++) {
        score += (countOfMyRoad[i] * scoreOfMyRoad[i] - countOfEnemyRoad[i] * scoreOfEnemyRoad[i]);
    }
    for (int i=4; i<=5; i++) {
        if (countOfMyRoad[i] >= 2 && countOfEnemyRoad[i] >= 2) {
            score += (countOfMyRoad[i] * scoreOfMyRoad[i+1] - countOfEnemyRoad[i] * scoreOfEnemyRoad[i+1]);
        } else if (countOfMyRoad[i] >= 2) {
            score += (countOfMyRoad[i] * scoreOfMyRoad[i+1] - countOfEnemyRoad[i] * scoreOfEnemyRoad[i]);
        } else if (countOfEnemyRoad[i] >= 2) {
            score += (countOfMyRoad[i] * scoreOfMyRoad[i] - countOfEnemyRoad[i] * scoreOfEnemyRoad[i+1]);
        } else {
            score += (countOfMyRoad[i] * scoreOfMyRoad[i] - countOfEnemyRoad[i] * scoreOfEnemyRoad[i]);
        }
    }
    score += (countOfMyRoad[6] * scoreOfMyRoad[6] - countOfEnemyRoad[6] * scoreOfEnemyRoad[6]);

    return score;
}


/**
 * 预评估
 */
long Evaluator::preEvaluate(Step step) {
    long score = 0;
    std::vector<int> countOfMyRoad = std::vector<int>(10, 0);
    std::vector<int> countOfEnemyRoad = std::vector<int>(10, 0);

    int maxCount;
    // 纵向扫描
    maxCount = 0;
    for (int x=step.x, y=step.y-5; y<GRID_SIZE-5 && y<=step.y; y++) {
        if (y < 0) continue;

        int count = getCount(x, y) + getCount(x, y + 1) + getCount(x, y + 2) + getCount(x, y + 3) + getCount(x, y + 4) + getCount(x, y + 5);

        if (count > 6 && count % 7 != 0) continue; // 黑白混杂不计入
        if (count > maxCount) maxCount = count; // 更新max
    }
    updateRoadTypeNum(maxCount, countOfMyRoad, countOfEnemyRoad);
    // 横向扫描
    maxCount = 0;
    for (int x=step.x-5, y=step.y; x<GRID_SIZE-5 && x<=step.x; x++) {
        if (x < 0) continue;

        int count = getCount(x, y) + getCount(x + 1, y) + getCount(x + 2, y) + getCount(x + 3, y) + getCount(x + 4, y) + getCount(x + 5, y);

        if (count > 6 && count % 7 != 0) continue; // 黑白混杂不计入
        if (count > maxCount) maxCount = count; // 更新max
    }
    updateRoadTypeNum(maxCount, countOfMyRoad, countOfEnemyRoad);
    // 右下对角扫描
    maxCount = 0;
    for (int x=step.x-5, y=step.y-5; (x<GRID_SIZE-5 && x<=step.x) && (y<GRID_SIZE-5 && y<=step.y); x++, y++) {
        if (x < 0 || y < 0) continue;

        int count = getCount(x, y) + getCount(x + 1, y + 1) + getCount(x + 2, y + 2) + getCount(x + 3, y + 3) + getCount(x + 4, y + 4) + getCount(x + 5, y + 5);

        if (count > 6 && count % 7 != 0) continue; // 黑白混杂不计入
        if (count > maxCount) maxCount = count; // 更新max
    }
    updateRoadTypeNum(maxCount, countOfMyRoad, countOfEnemyRoad);
    // 左下对角扫描
    maxCount = 0;
    for (int x=step.x+5, y=step.y-5; (x>=5 && x>=step.x) && (y<GRID_SIZE-5 && y<=step.y); x--, y++) {
        if (x >= GRID_SIZE || y < 0) continue;

        int count = getCount(x, y) + getCount(x - 1, y + 1) + getCount(x - 2, y + 2) + getCount(x - 3, y + 3) + getCount(x - 4, y + 4) + getCount(x - 5, y + 5);

        if (count > 6 && count % 7 != 0) continue; // 黑白混杂不计入
        if (count > maxCount) maxCount = count; // 更新max
    }
    updateRoadTypeNum(maxCount, countOfMyRoad, countOfEnemyRoad);

    //
    for (int i=1; i<=2; i++) {
        score += (countOfMyRoad[i] * scoreOfMyRoad[i] - countOfEnemyRoad[i] * scoreOfEnemyRoad[i]);
    }
    for (int i=3; i<=5; i++) {
        if (countOfMyRoad[i] >= 2 && countOfEnemyRoad[i] >= 2) {
            score += (countOfMyRoad[i] * scoreOfMyRoad[i+1] - countOfEnemyRoad[i] * scoreOfEnemyRoad[i+1]);
        } else if (countOfMyRoad[i] >= 2) {
            score += (countOfMyRoad[i] * scoreOfMyRoad[i+1] - countOfEnemyRoad[i] * scoreOfEnemyRoad[i]);
        } else if (countOfEnemyRoad[i] >= 2) {
            score += (countOfMyRoad[i] * scoreOfMyRoad[i] - countOfEnemyRoad[i] * scoreOfEnemyRoad[i+1]);
        } else {
            score += (countOfMyRoad[i] * scoreOfMyRoad[i] - countOfEnemyRoad[i] * scoreOfEnemyRoad[i]);
        }
    }
    score += (countOfMyRoad[6] * scoreOfMyRoad[6] - countOfEnemyRoad[6] * scoreOfEnemyRoad[6]);

    return score;
}