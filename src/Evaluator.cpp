#include "Evaluator.h"

Evaluator::Evaluator(Color botColor, Grid grid): currentColor(botColor), grid(grid) {
    //防守型
    scoreOfMyRoad[0][1] = 1;
    scoreOfMyRoad[0][2] = 5;
    scoreOfMyRoad[0][3] = 10;
    scoreOfMyRoad[0][4] = 25;
    scoreOfMyRoad[0][5] = 25;
    scoreOfMyRoad[0][6] = 10000;
    scoreOfEnemyRoad[0][1] = 1;
    scoreOfEnemyRoad[0][2] = 5;
    scoreOfEnemyRoad[0][3] = 15;
    scoreOfEnemyRoad[0][4] = 35;
    scoreOfEnemyRoad[0][5] = 25;
    scoreOfEnemyRoad[0][6] = 10000;

    //进攻型
    scoreOfMyRoad[1][1] = 1;
    scoreOfMyRoad[1][2] = 15;
    scoreOfMyRoad[1][3] = 30;
    scoreOfMyRoad[1][4] = 50;
    scoreOfMyRoad[1][5] = 50;
    scoreOfMyRoad[1][6] = 10000;
    scoreOfEnemyRoad[1][1] = 1;
    scoreOfEnemyRoad[1][2] = 10;
    scoreOfEnemyRoad[1][3] = 15;
    scoreOfEnemyRoad[1][4] = 35;
    scoreOfEnemyRoad[1][5] = 25;
    scoreOfEnemyRoad[1][6] = 10000;
}

/**
 * 所下棋子的价值：下棋后局面分数 - 下棋前局面分数
 */
int Evaluator::evaluate(Turn move) {
    baseScore = calScore();

    int fScore = calScore(move);

    grid.doStep(move.x0, move.y0, currentColor);
    grid.doStep(move.x1, move.y1, currentColor);

    int bScore = calScore(move);

    return bScore - fScore;
}

/**
 * 计数
 * 黑棋加一分，白棋加7分
 */
inline int Evaluator::getCount(int x, int y) {
    return (grid.data[x][y] == WHITE) ? 7 : grid.data[x][y];
}


/**
 * 更新不同路类型的数量
 */
inline void Evaluator::updateRoadTypeNum(int count, std::vector<int>& countOfMyRoad, std::vector<int>& countOfEnemyRoad) const {
    // 没棋 或 黑白混杂
    if (count == 0 || (count > 6 && count % 7 != 0)) return;

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
 * 计算此时局面的总分数，得到baseScore(全局扫描)
 */
int Evaluator::calScore() {
    int score = 0;
    int condition = 0; // condition用于表示进攻型还是防守型, 1进攻, 0防守
    std::vector<int> countOfMyRoad = std::vector<int>(10, 0);
    std::vector<int> countOfEnemyRoad = std::vector<int>(10, 0);
    
    if (baseScore > ALERT_SCORE) {
        condition = 1;
    }

    // 纵向扫描
    for (int x=0; x<GRID_SIZE-5; x++) {
        for (int y=0; y<GRID_SIZE; y++) {
            int count = getCount(x, y) + getCount(x + 1, y) + getCount(x + 2, y) + getCount(x + 3, y) + getCount(x + 4, y) + getCount(x + 5, y);
            updateRoadTypeNum(count, countOfMyRoad, countOfEnemyRoad);
        }
    }
    // 横向扫描
    for (int x=0; x<GRID_SIZE; x++) {
        for (int y=0; y<GRID_SIZE-5; y++) {
            int count = getCount(x, y) + getCount(x, y + 1) + getCount(x, y + 2) + getCount(x, y + 3) + getCount(x, y + 4) + getCount(x, y + 5);
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
    for (int x=0; x<GRID_SIZE-5; x++) {
        for (int y=5; y<GRID_SIZE; y++) {
            int count = getCount(x, y) + getCount(x + 1, y - 1) + getCount(x + 2, y - 2) + getCount(x + 3, y - 3) + getCount(x + 4, y - 4) + getCount(x + 5, y - 5);
            updateRoadTypeNum(count, countOfMyRoad, countOfEnemyRoad);
        }
    }

    //
    for (int i=1; i<=6; i++) {
        score += (countOfMyRoad[i] * scoreOfMyRoad[condition][i] - countOfEnemyRoad[i] * scoreOfEnemyRoad[condition][i]);
    }
    
    return score;
}


/**
 * 计算此时局面的总分数(局部扫描)
 */
int Evaluator::calScore(Turn move) {
    int score = 0;
    int condition = 0; // condition用于表示进攻型还是防守型, 1进攻, 0防守
    int x0 = move.x0, y0 = move.y0, x1 = move.x1, y1 = move.y1;
    std::vector<int> countOfMyRoad = std::vector<int>(10, 0);
    std::vector<int> countOfEnemyRoad = std::vector<int>(10, 0);

    if (baseScore > ALERT_SCORE) {
        condition = 1;
    }

    // 纵向扫描
    for (int x=x0-5, y=y0; x<GRID_SIZE-5 && x<=x0; x++) {
        if (x < 0) continue;

        int count = getCount(x, y) + getCount(x + 1, y) + getCount(x + 2, y) + getCount(x + 3, y) + getCount(x + 4, y) + getCount(x + 5, y);
        updateRoadTypeNum(count, countOfMyRoad, countOfEnemyRoad);
    }
    for (int x=x1-5, y=y1; x<GRID_SIZE-5 && x<=x1; x++) {
        if (x < 0) continue;
        if (x == x0 || x + 1 == x0 || x + 2 == x0 || x + 3 == x0 || x + 4 == x0 || x + 5 == x0) continue; // 跳过重复扫描

        int count = getCount(x, y) + getCount(x + 1, y) + getCount(x + 2, y) + getCount(x + 3, y) + getCount(x + 4, y) + getCount(x + 5, y);
        updateRoadTypeNum(count, countOfMyRoad, countOfEnemyRoad);
    }
    // 横向扫描
    for (int x=x0, y=y0-5; y<GRID_SIZE-5 && y<=y0; y++) {
        if (y < 0) continue;

        int count = getCount(x, y) + getCount(x, y + 1) + getCount(x, y + 2) + getCount(x, y + 3) + getCount(x, y + 4) + getCount(x, y + 5);
        updateRoadTypeNum(count, countOfMyRoad, countOfEnemyRoad);
    }
    for (int x=x1, y=y1-5; y<GRID_SIZE-5 && y<=y1; y++) {
        if (y < 0) continue;
        if (y == y0 || y + 1 == y0 || y + 2 == y0 || y + 3 == y0 || y + 4 == y0 || y + 5 == y0) continue; // 跳过重复扫描

        int count = getCount(x, y) + getCount(x, y + 1) + getCount(x, y + 2) + getCount(x, y + 3) + getCount(x, y + 4) + getCount(x, y + 5);
        updateRoadTypeNum(count, countOfMyRoad, countOfEnemyRoad);
    }
    // 右下对角扫描
    for (int x=x0-5, y=y0+5; (x<GRID_SIZE-5 && x<=x0) && (y>=y0); x++, y--) {
        if (x < 0 || y > GRID_SIZE) continue;

        int count = getCount(x, y) + getCount(x + 1, y - 1) + + getCount(x + 2, y - 2) + + getCount(x + 3, y - 3) + + getCount(x + 4, y - 4) + + getCount(x + 5, y - 5);
        updateRoadTypeNum(count, countOfMyRoad, countOfEnemyRoad);
    }
    for (int x=x1-5, y=y1+5; (x<GRID_SIZE-5 && x<=x1) && (y>=y1); x++, y--) {
        if (x < 0 || y > GRID_SIZE) continue;
        if ((x == x0 && y == y0) || (x + 1 == x0 && y - 1 == y0) || (x + 2 == x0 && y - 2 == y0)|| (x + 3 == x0 && y - 3 == y0) || (x + 4 == x0 && y - 4 == y0) || (x + 5 == x0 && y - 5 == y0)) continue; // 跳过重复扫描

        int count = getCount(x, y) + getCount(x + 1, y - 1) + + getCount(x + 2, y - 2) + + getCount(x + 3, y - 3) + + getCount(x + 4, y - 4) + + getCount(x + 5, y - 5);
        updateRoadTypeNum(count, countOfMyRoad, countOfEnemyRoad);
    }
    // 左下对角扫描
    for (int x=x0-5, y=y0-5; (x<GRID_SIZE-5 && x<=x0) && (y<GRID_SIZE-5 && y<=y0); x++, y++) {
        if (x < 0 || y < 0) continue;

        int count = getCount(x, y) + getCount(x + 1, y + 1) + + getCount(x + 2, y + 2) + + getCount(x + 3, y + 3) + + getCount(x + 4, y + 4) + + getCount(x + 5, y + 5);
        updateRoadTypeNum(count, countOfMyRoad, countOfEnemyRoad);
    }
    for (int x=x1-5, y=y1-5; (x<GRID_SIZE-5 && x<=x1) && (y<GRID_SIZE-5 && y<=y1); x++, y++) {
        if (x < 0 || y < 0) continue;
        if ((x == x0 && y == y0) || (x + 1 == x0 && y + 1 == y0) || (x + 2 == x0 && y + 2 == y0)|| (x + 3 == x0 && y + 3 == y0) || (x + 4 == x0 && y + 4 == y0) || (x + 5 == x0 && y + 5 == y0)) continue; // 跳过重复扫描

        int count = getCount(x, y) + getCount(x + 1, y + 1) + + getCount(x + 2, y + 2) + + getCount(x + 3, y + 3) + + getCount(x + 4, y + 4) + + getCount(x + 5, y + 5);
        updateRoadTypeNum(count, countOfMyRoad, countOfEnemyRoad);
    }

    //
    for(int i = 1; i < 7; i++) {
        score += (countOfMyRoad[i] * scoreOfMyRoad[condition][i] - countOfEnemyRoad[i] * scoreOfEnemyRoad[condition][i]);
    }

    return score;
}
