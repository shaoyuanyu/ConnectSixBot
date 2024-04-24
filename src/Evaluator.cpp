#include <iostream>
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
    long score = 0;
    std::vector<int> countOfMyRoad = std::vector<int>(10, 0);
    std::vector<int> countOfEnemyRoad = std::vector<int>(10, 0);
    int maxRoad;

    // 纵向
    for (int x=0; x<GRID_SIZE; x++) {
        for (int y=0; y<GRID_SIZE-5; y++) {
            maxRoad = 0;
            while (y < GRID_SIZE - 5) {
                int road = grid.road[y][x][0];

                if (road >= 7 && road % 7 != 0) break; // 混色路不计入

                if (road > maxRoad) maxRoad = road;

                y++;
            }
            updateRoadCount(maxRoad, countOfMyRoad, countOfEnemyRoad);
        }
    }

    // output
//    std::cout << "扫描纵向后" << std::endl;
//    for (int i=1; i<=6; i++) {
//        std::cout << i << "路: " << countOfMyRoad[i] << ", " << countOfEnemyRoad[i] << std::endl;
//    }
//    std::cout << std::endl;

    // 横向
    for (int y=0; y<GRID_SIZE; y++) {
        for (int x=0; x<GRID_SIZE-5; x++) {
            maxRoad = 0;
            while (x < GRID_SIZE - 5) {
                int road = grid.road[y][x][1];

                if (road >= 7 && road % 7 != 0) break; // 混色路不计入

                if (road > maxRoad) maxRoad = road;

                x++;
            }
            updateRoadCount(maxRoad, countOfMyRoad, countOfEnemyRoad);
        }
    }

    // output
//    std::cout << "扫描横向后" << std::endl;
//    for (int i=1; i<=6; i++) {
//        std::cout << i << "路: " << countOfMyRoad[i] << ", " << countOfEnemyRoad[i] << std::endl;
//    }
//    std::cout << std::endl;

    // 右下对角
    for (int startX=0, startY=0; startY<GRID_SIZE-5; startY++) {
        for (int x=startX, y=startY; x<GRID_SIZE-5 && y<GRID_SIZE-5; x++, y++) {
            maxRoad = 0;
            while (x < GRID_SIZE - 5 && y < GRID_SIZE - 5) {
                int road = grid.road[y][x][2];

                if (road >= 7 && road % 7 != 0) break; // 混色路不计入

                if (road > maxRoad) maxRoad = road;

                x++, y++;
            }
            updateRoadCount(maxRoad, countOfMyRoad, countOfEnemyRoad);
        }
    }
    for (int startX=1, startY=0; startX<GRID_SIZE-5; startX++) {
        for (int x=startX, y=startY; x<GRID_SIZE-5 && y<GRID_SIZE-5; x++, y++) {
            maxRoad = 0;
            while (x < GRID_SIZE - 5 && y < GRID_SIZE - 5) {
                int road = grid.road[y][x][2];

                if (road >= 7 && road % 7 != 0) break; // 混色路不计入

                if (road > maxRoad) maxRoad = road;

                x++, y++;
            }
            updateRoadCount(maxRoad, countOfMyRoad, countOfEnemyRoad);
        }
    }

    // output
//    std::cout << "扫描右下对角后" << std::endl;
//    for (int i=1; i<=6; i++) {
//        std::cout << i << "路: " << countOfMyRoad[i] << ", " << countOfEnemyRoad[i] << std::endl;
//    }
//    std::cout << std::endl;

    // 左下对角
    for (int startX=GRID_SIZE-1, startY=0; startY<GRID_SIZE-5; startY++) {
        for (int x=startX, y=startY; x>=5 && y<GRID_SIZE-5; x--, y++) {
            maxRoad = 0;
            while (x >= 5 && y < GRID_SIZE - 5) {
                int road = grid.road[y][x][3];

                if (road >= 7 && road % 7 != 0) break; // 混色路不计入

                if (road > maxRoad) maxRoad = road;

                x--, y++;
            }
            updateRoadCount(maxRoad, countOfMyRoad, countOfEnemyRoad);
        }
    }
    for (int startX=GRID_SIZE-2, startY=0; startX>=5; startX--) {
        for (int x=startX, y=startY; x>=5 && y<GRID_SIZE-5; x--, y++) {
            maxRoad = 0;
            while (x >= 5 && y < GRID_SIZE - 5) {
                int road = grid.road[y][x][3];

                if (road >= 7 && road % 7 != 0) break; // 混色路不计入

                if (road > maxRoad) maxRoad = road;

                x--, y++;
            }
//            std::cout << "TEST: rd: (" << x << ", " << y << "): " << maxRoad << std::endl;
            updateRoadCount(maxRoad, countOfMyRoad, countOfEnemyRoad);
        }
    }

    // output
//    std::cout << "扫描左下对角后" << std::endl;
//    for (int i=1; i<=6; i++) {
//        std::cout << i << "路: " << countOfMyRoad[i] << ", " << countOfEnemyRoad[i] << std::endl;
//    }
//    std::cout << std::endl;

    // 计分
    for (int i=1; i<=6; i++) {
        score += (countOfMyRoad[i] * scoreOfMyRoad[i] - countOfEnemyRoad[i] * scoreOfEnemyRoad[i]);
    }

    return score;
}


/**
 * 局部评估
 */
long Evaluator::evaluate(Move move) {
    long score = 0;
    std::vector<int> countOfMyRoad = std::vector<int>(10, 0);
    std::vector<int> countOfEnemyRoad = std::vector<int>(10, 0);
    int maxRoad;
    bool inOneRoad;

    // 纵向 (x0, y0)
    maxRoad = 0;
    for (int x=move.x0, y=move.y0-5; y<=move.y0; y++) {
        if (y < 0) continue;

        int road = grid.road[y][x][0];

        if (road >= 7 && road % 7 != 0) continue; // 混色路不计入

        if (road > maxRoad) maxRoad = road;
    }
    updateRoadCount(maxRoad, countOfMyRoad, countOfEnemyRoad);

    // 纵向 (x1, y1)
    inOneRoad = false;
    if (move.x0 == move.x1 && abs(move.y0 - move.y1) <= 5) {
        inOneRoad = true;
        for (int x=move.x0, y=((move.y0<move.y1)?move.y0:move.y1); y<=((move.y0>move.y1)?move.y0:move.y1); y++) {
            if (grid.data[y][x] != grid.data[move.y0][move.x0]) {
                inOneRoad = false;
                break;
            }
        }
    }
    if (!inOneRoad) {
        maxRoad = 0;
        for (int x=move.x1, y=move.y1-5; y<=move.y1; y++) {
            if (y < 0) continue;

            int road = grid.road[y][x][0];

            if (road >= 7 && road % 7 != 0) continue; // 混色路不计入

            if (road > maxRoad) maxRoad = road;
        }
        updateRoadCount(maxRoad, countOfMyRoad, countOfEnemyRoad);
    }

    // output
//    std::cout << "扫描纵向后" << std::endl;
//    for (int i=1; i<=6; i++) {
//        std::cout << i << "路: " << countOfMyRoad[i] << ", " << countOfEnemyRoad[i] << std::endl;
//    }
//    std::cout << std::endl;

    // 横向 (x0, y0)
    maxRoad = 0;
    for (int x=move.x0-5, y=move.y0; x<=move.x0; x++) {
        if (x < 0) continue;

        int road = grid.road[y][x][1];

        if (road >= 7 && road % 7 != 0) continue; // 混色路不计入

        if (road > maxRoad) maxRoad = road;
    }
    updateRoadCount(maxRoad, countOfMyRoad, countOfEnemyRoad);

    // 横向 (x1, y1)
    inOneRoad = false;
    if (move.y0 == move.y1 && abs(move.x0 - move.x1) <= 5) {
        inOneRoad = true;
        for (int x=((move.x0<move.x1)?move.x0:move.x1), y=move.y0; x<=((move.x0>move.x1)?move.x0:move.x1); x++) {
            if (grid.data[y][x] != grid.data[move.y0][move.x0]) {
                inOneRoad = false;
                break;
            }
        }
    }
    if (!inOneRoad) {
        maxRoad = 0;
        for (int x=move.x1-5, y=move.y1; x<=move.x1; x++) {
            if (x < 0) continue;

            int road = grid.road[y][x][1];

            if (road >= 7 && road % 7 != 0) continue; // 混色路不计入

            if (road > maxRoad) maxRoad = road;
        }
        updateRoadCount(maxRoad, countOfMyRoad, countOfEnemyRoad);
    }

    // output
//    std::cout << "扫描横向后" << std::endl;
//    for (int i=1; i<=6; i++) {
//        std::cout << i << "路: " << countOfMyRoad[i] << ", " << countOfEnemyRoad[i] << std::endl;
//    }
//    std::cout << std::endl;

    // 右下对角 (x0, y0)
    maxRoad = 0;
    for (int x=move.x0-5, y=move.y0-5; x<=move.x0 && y<=move.y0; x++, y++) {
        if (x < 0 || y < 0) continue;

        int road = grid.road[y][x][2];

        if (road >= 7 && road % 7 != 0) continue; // 混色路不计入

        if (road > maxRoad) maxRoad = road;
    }
    updateRoadCount(maxRoad, countOfMyRoad, countOfEnemyRoad);

    // 右下对角 (x1, y1)
    inOneRoad = false;
    if (move.x0 - move.x1 == move.y0 - move.y1 && abs(move.x0-move.x1) <= 5) {
        inOneRoad = true;
        for (int x=((move.x0<move.x1)?move.x0:move.x1), y=((move.y0<move.y1)?move.y0:move.y1); x<=((move.x0>move.x1)?move.x0:move.x1) && y<=((move.y0>move.y1)?move.y0:move.y1); x++, y++) {
            if (grid.data[y][x] != grid.data[move.y0][move.x0]) {
                inOneRoad = false;
                break;
            }
        }
    }
    if (!inOneRoad) {
        maxRoad = 0;
        for (int x=move.x1-5, y=move.y1-5; x<=move.x1 && y<=move.y1; x++, y++) {
            if (x < 0 || y < 0) continue;

            int road = grid.road[y][x][2];

            if (road >= 7 && road % 7 != 0) continue; // 混色路不计入

            if (road > maxRoad) maxRoad = road;
        }
        updateRoadCount(maxRoad, countOfMyRoad, countOfEnemyRoad);
    }

    // output
//    std::cout << "扫描右下对角后" << std::endl;
//    for (int i=1; i<=6; i++) {
//        std::cout << i << "路: " << countOfMyRoad[i] << ", " << countOfEnemyRoad[i] << std::endl;
//    }
//    std::cout << std::endl;

    // 左下对角 (x0, y0)
    maxRoad = 0;
    for (int x=move.x0+5, y=move.y0-5; x>=move.x0 && y<=move.y0; x--, y++) {
        if (x >= GRID_SIZE || y < 0) continue;

        int road = grid.road[y][x][3];

        if (road >= 7 && road % 7 != 0) continue; // 混色路不计入

        if (road > maxRoad) maxRoad = road;
    }
    updateRoadCount(maxRoad, countOfMyRoad, countOfEnemyRoad);

    // 左下对角 (x1, y1)
    inOneRoad = false;
    if (move.x0 - move.x1 == move.y1 - move.y0 && abs(move.x0-move.x1) <= 5) {
        inOneRoad = true;
        for (int x=((move.x0>move.x1)?move.x0:move.x1), y=((move.y0<move.y1)?move.y0:move.y1); x>=((move.x0<move.x1)?move.x0:move.x1) && y<=((move.y0>move.y1)?move.y0:move.y1); x--, y++) {
            if (grid.data[y][x] != grid.data[move.y0][move.x0]) {
                inOneRoad = false;
                break;
            }
        }
    }
    if (!inOneRoad) {
        maxRoad = 0;
        for (int x=move.x0+5, y=move.y0-5; x>=move.x0 && y<=move.y0; x--, y++) {
            if (x >= GRID_SIZE || y < 0) continue;

            int road = grid.road[y][x][3];

            if (road >= 7 && road % 7 != 0) continue; // 混色路不计入

            if (road > maxRoad) maxRoad = road;
        }
        updateRoadCount(maxRoad, countOfMyRoad, countOfEnemyRoad);
    }

    // output
//    std::cout << "扫描左下对角后" << std::endl;
//    for (int i=1; i<=6; i++) {
//        std::cout << i << "路: " << countOfMyRoad[i] << ", " << countOfEnemyRoad[i] << std::endl;
//    }
//    std::cout << std::endl;


//    // 双四路 = 六路
//    if (countOfMyRoad[4] >= 2) {
//        countOfMyRoad[6] = countOfMyRoad[4] / 2;
//        countOfMyRoad[4] %= 2;
//    }
//    if (countOfEnemyRoad[4] >= 2) {
//        countOfEnemyRoad[6] = countOfEnemyRoad[4] / 2;
//        countOfEnemyRoad[6] %= 2;
//    }
//
//    // 四路 + 五路 = 六路
//    if (countOfMyRoad[4] > 0 && countOfMyRoad[5] > 0) {
//        int pairs = (countOfMyRoad[4] <= countOfMyRoad[5]) ? countOfMyRoad[4] : countOfMyRoad[5];
//        countOfMyRoad[4] -= pairs;
//        countOfMyRoad[5] -= pairs;
//        countOfMyRoad[6] += pairs;
//    }
//    if (countOfEnemyRoad[4] > 0 && countOfEnemyRoad[5] > 0) {
//        int pairs = (countOfEnemyRoad[4] <= countOfEnemyRoad[5]) ? countOfEnemyRoad[4] : countOfEnemyRoad[5];
//        countOfEnemyRoad[4] -= pairs;
//        countOfEnemyRoad[5] -= pairs;
//        countOfEnemyRoad[6] += pairs;
//    }
//
//    // 双五路 = 六路
//    if (countOfMyRoad[5] >= 2) {
//        countOfMyRoad[6] = countOfMyRoad[5] / 2;
//        countOfMyRoad[5] %= 2;
//    }
//    if (countOfEnemyRoad[5] >= 2) {
//        countOfEnemyRoad[6] = countOfEnemyRoad[5] / 2;
//        countOfEnemyRoad[5] %= 2;
//    }


    // 计分
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
    int maxRoad;

    // 纵向
    maxRoad = 0;
    for (int x=step.x, y=step.y-5; y<=step.y; y++) {
        if (y < 0) continue;

        int road = grid.road[y][x][0];

        if (road >= 7 && road % 7 != 0) continue; // 混色路不计入

        if (road > maxRoad) maxRoad = road;
    }
    updateRoadCount(maxRoad, countOfMyRoad, countOfEnemyRoad);


    // 横向
    maxRoad = 0;
    for (int x=step.x-5, y=step.y; x<=step.x; x++) {
        if (x < 0) continue;

        int road = grid.road[y][x][1];

        if (road >= 7 && road % 7 != 0) continue; // 混色路不计入

        if (road > maxRoad) maxRoad = road;
    }
    updateRoadCount(maxRoad, countOfMyRoad, countOfEnemyRoad);


    // 右下对角
    maxRoad = 0;
    for (int x=step.x-5, y=step.y-5; x<=step.x && y<=step.y; x++, y++) {
        if (x < 0 || y < 0) continue;

        int road = grid.road[y][x][2];

        if (road >= 7 && road % 7 != 0) continue; // 混色路不计入

        if (road > maxRoad) maxRoad = road;
    }
    updateRoadCount(maxRoad, countOfMyRoad, countOfEnemyRoad);


    // 左下对角
    maxRoad = 0;
    for (int x=step.x+5, y=step.y-5; x>=step.x && y<=step.y; x--, y++) {
        if (x >= GRID_SIZE || y < 0) continue;

        int road = grid.road[y][x][3];

        if (road >= 7 && road % 7 != 0) continue; // 混色路不计入

        if (road > maxRoad) maxRoad = road;
    }
    updateRoadCount(maxRoad, countOfMyRoad, countOfEnemyRoad);

    // Step模式 五路 = 六路
    countOfMyRoad[6] += countOfMyRoad[5];
    countOfMyRoad[5] = 0;

    // 计分
    for (int i=1; i<=6; i++) {
        score += countOfMyRoad[i] * scoreOfMyRoad[i];
    }

    return score;
}


/**
 *
 */
inline void Evaluator::updateRoadCount(int maxRoad, std::vector<int> &countOfMyRoad, std::vector<int> &countOfEnemyRoad) {
    Color roadColor = BLACK;

    if (maxRoad > 0) {
        // 判断白色路
        if (maxRoad >= 7) {
            roadColor = WHITE;
            maxRoad /= 7;
        }
        // 计数
        if (roadColor == botColor) countOfMyRoad[maxRoad]++;
        else countOfEnemyRoad[maxRoad]++;
    }
}
