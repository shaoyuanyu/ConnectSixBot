#include "Evaluator.h"

/**
 * Evaluator构造方法
 */
Evaluator::Evaluator(Grid grid, Color currentColor): grid(grid), currentColor(currentColor) {
    scoreOfMyRoad[0] = 0;
    scoreOfMyRoad[1] = 2;
    scoreOfMyRoad[2] = 200;
    scoreOfMyRoad[3] = 20000;
    scoreOfMyRoad[4] = 2000000;
    scoreOfMyRoad[5] = 200000000;
    scoreOfMyRoad[6] = 200000000000;

    scoreOfEnemyRoad[0] = 0;
    scoreOfEnemyRoad[1] = 1;
    scoreOfEnemyRoad[2] = 100;
    scoreOfEnemyRoad[3] = 10000;
    scoreOfEnemyRoad[4] = 1000000;
    scoreOfEnemyRoad[5] = 100000000;
    scoreOfEnemyRoad[6] = 100000000000;
}


/**
 * 所下棋子的价值：下棋后局面分数 - 下棋前局面分数
 */
long Evaluator::evaluate() {
    return calScore();
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
            updateRoadTypeNum(count, countOfMyRoad, countOfEnemyRoad);
        }
    }
    // 横向扫描
    for (int x=0; x<GRID_SIZE-5; x++) {
        for (int y=0; y<GRID_SIZE; y++) {
            int count = getCount(x, y) + getCount(x + 1, y) + getCount(x + 2, y) + getCount(x + 3, y) + getCount(x + 4, y) + getCount(x + 5, y);
            updateRoadTypeNum(count, countOfMyRoad, countOfEnemyRoad);
        }
    }
    // 右下对角扫描
    for (int x=0; x<GRID_SIZE-5; x++) {
        for (int y=0; y<GRID_SIZE-5; y++) {
            int count = getCount(x, y) + getCount(x + 1, y + 1) + getCount(x + 2, y + 2) + getCount(x + 3, y + 3) + getCount(x + 4, y + 4) + getCount(x + 5, y + 5);
            updateRoadTypeNum(count, countOfMyRoad, countOfEnemyRoad);
        }
    }
    // 左下对角扫描
    for (int x=GRID_SIZE-1; x>=5; x--) {
        for (int y=0; y<GRID_SIZE-5; y++) {
            int count = getCount(x, y) + getCount(x - 1, y + 1) + getCount(x - 2, y + 2) + getCount(x - 3, y + 3) + getCount(x - 4, y + 4) + getCount(x - 5, y + 5);
            updateRoadTypeNum(count, countOfMyRoad, countOfEnemyRoad);
        }
    }

    //
    for (int i=1; i<=6; i++) {
        score += (countOfMyRoad[i] * scoreOfMyRoad[i] - countOfEnemyRoad[i] * scoreOfEnemyRoad[i]);
    }
    
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
    for(int i = 1; i < 7; i++) {
        score += (countOfMyRoad[i] * scoreOfMyRoad[i] - countOfEnemyRoad[i] * scoreOfEnemyRoad[i]);
    }

    return score;
}