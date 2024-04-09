//
// ysy
// 2024/3/28, 29
//

#include "Bot.h"
#include <iostream>
#include <cmath>


void Bot::setColor(Color c) {
    botColor = c;
}


Color Bot::getColor() {
    return botColor;
}


Color Bot::getOppositeColor() {
    return -botColor;
}


/**
 * 基于开局库进行开局决策
 */
Turn Bot::makeOpening() {
    return Turn(7, 7, -1, -1);
}


/**
 * 基于博弈树进行决策
 */
Turn Bot::makeDecision(Grid& grid, int turnId) {
    // 我方为黑开局
    if (turnId == 1 && botColor == BLACK) return makeOpening();

    // 模拟落子构建博弈树
    simulateStep(root, grid, std::vector<Turn>(), -botColor, 0, basicDepthLimit);

    std::cout << "find max..." << std::endl;

    Turn nextTurn = root->children[0]->turn;
    float max = root->children[0]->score;

    for (GameNode* child: root->children) {
        std::cout << "(" << child->turn.x0 << ", " << child->turn.y0 << ")" << ", (" << child->turn.x1 << ", " << child->turn.y1 << "): " << child->score << std::endl;

        if (child->score > max) {
            max = child->score;
            nextTurn = child->turn;
        }
    }

    return nextTurn;
}


/**
 * 模拟走步
 * 递归建立博弈树，在叶节点调用评估函数，反向传播评估得分
 */
float Bot::simulateStep(GameNode*& currentNode, const Grid& currentGrid, const std::vector<Turn>& preTurns, const Color currentColor, int turnCount, int currentDepthLimit) {
    if (turnCount == 1) {
        int weight0 = currentGrid.weight[currentNode->turn.x0][currentNode->turn.y0];
        int weight1 = currentGrid.weight[currentNode->turn.x1][currentNode->turn.y1];
        currentDepthLimit = basicDepthLimit + (weight0+weight1)/2;
    }

    // 若搜索深度触底，终止搜索，进行评估
    if (turnCount == currentDepthLimit) {
        for (Turn preTurn: preTurns) {
            currentNode->score += evaluate(currentGrid, Step(preTurn.x0, preTurn.y0));
            currentNode->score += evaluate(currentGrid, Step(preTurn.x1, preTurn.y1));
        }

        // 调试输出
//        std::cout << "step " << turnCount << ": " << "(" << currentNode->turn.x0 << ", " << currentNode->turn.y0 << ")" << ", (" << currentNode->turn.x1 << ", " << currentNode->turn.y1 << ")" << std::endl;
//        std::cout << "isMaxNode: " << currentNode->isMaxNode << std::endl;
//        std::cout << "evaluation: " << currentNode->score << std::endl << std::endl;

        return currentNode->score;
    }

    // 最大值，最小值
    float max = FLT_MIN, min = FLT_MAX;

    // 继续搜索
    for (int x0=0; x0<GRID_SIZE; x0++) {
        for (int y0=0; y0<GRID_SIZE; y0++) {
            // 第一手落子位置非空
            if (currentGrid.grid[x0][y0] != BLANK) continue;
            // 第一手落子位置过于疏松
            if (currentGrid.weight[x0][y0] == 0) continue;

            // 寻找第二手落子
            for (int x1 = x0; x1 < GRID_SIZE; x1++) {
                // 确定y1初始值
                int y1Start = 0;
                if (x1 == x0) {
                    if (y0 == GRID_SIZE - 1) continue;
                    y1Start = y0 + 1;
                }

                for (int y1=y1Start; y1<GRID_SIZE; y1++) {
                    // 第二手落子位置非空
                    if (currentGrid.grid[x1][y1] != BLANK) continue;
                    // 第二手落子位置过于疏松
                    if (currentGrid.weight[x1][y1] == 0) continue;

                    // alpha-beta检查
                    if (currentNode->alpha < currentNode->beta) {
//                        std::cout << "turn " << turnCount << ": (" << currentNode->turn.x0 << ", " << currentNode->turn.y0 << "), (" << currentNode->turn.x1 << ", " << currentNode->turn.y1 << ") " << "alpha-beta cut." << std::endl;
                        currentNode->score = (currentNode->isMaxNode) ? max : min;
                        return currentNode->score;
                    } else if (currentNode->alpha == currentNode->beta) {
                        currentNode->score = currentNode->alpha;
                        return currentNode->score;
                    }

                    // child node
                    auto* child = new GameNode(Turn(x0, y0, x1, y1), -currentColor == botColor, currentNode->alpha, currentNode->beta);
                    if (turnCount == 0) {
                        currentNode->addChild(child);
                    }
                    //
                    Grid newGrid = currentGrid;
                    newGrid.doStep(x0, y0, currentColor);
                    newGrid.doStep(x1, y1, currentColor);
                    //
                    std::vector<Turn> newTurns = preTurns;
                    newTurns.push_back(child->turn);

                    // 继续搜索
                    float childScore = simulateStep(child, newGrid, newTurns, -currentColor, turnCount + 1, currentDepthLimit);

                    // 反向传递
                    if (currentNode->isMaxNode && childScore > max) {
                        max = childScore;
                    } else if (!currentNode->isMaxNode && childScore < min) {
                        min = childScore;
                    }

                    // alpha-beta更新
                    if (currentNode->isMaxNode) {
                        if (childScore > currentNode->beta) {
                            currentNode->beta = childScore;
                        }
                    } else {
                        if (childScore < currentNode->alpha) {
                            currentNode->alpha = childScore;
                        }
                    }

                    // 调试输出
                    if (turnCount == 0) {
                        std::cout << "turn " << turnCount + 1 << ": " << "(" << child->turn.x0 << ", " << child->turn.y0 << ")" << ", (" << child->turn.x1 << ", " << child->turn.y1 << ")" << std::endl;
                        std::cout << "isMaxNode: " << currentNode->isMaxNode << std::endl;
                        std::cout << "score: " << childScore << std::endl;
                        std::cout << "alpha: " << currentNode->alpha << ", beta: " << currentNode->beta << std::endl;
                        std::cout << std::endl;
                    }

                    //
                    if (turnCount >= 1) {
                        delete child;
                    }
                }
            }
        }
    }

    currentNode->score = (currentNode->isMaxNode) ? max : min;

    return currentNode->score;
}



/**
 * 计数，同色+1,异色-1并终止，空位记为0.2
 */
inline bool Bot::count(const Grid& grid, const int& x, const int& y, const Color& currentColor, float& count) {
    if (x<0 || x>=GRID_SIZE || y<0 || y>=GRID_SIZE) return false;

    if (grid.grid[x][y] == currentColor) {
        // 同色
        count += 1;
    } else if (grid.grid[x][y] == -currentColor) {
        // 遇异色终止计数
        count += -1;
        return false;
    } else {
        // 空位记为0.2
        count += 0.2;
    }

    return true;
}


/**
 * 评估函数
 */
float Bot::evaluate(Grid grid, Step step) {
    Color currentColor = grid.grid[step.x][step.y];
    int x, y;
    float inLineCount = 1, inColumnCount = 1, inLeftDiagonalCount = 1, inRightDiagonalCount = 1;
    bool isLineUnbroken = true, isColumnUnbroken = true, isLeftDiagonalUnbroken = true, isRightDiagonalUnbroken = true;

    for (int shift = -1; shift >= -5; shift--) {
        // 行
        if (isLineUnbroken) {
            x = step.x;
            y = step.y + shift;
            isLineUnbroken = count(grid, x, y, currentColor, inLineCount);
        }

        // 列
        if (isColumnUnbroken) {
            x = step.x + shift;
            y = step.y;
            isColumnUnbroken = count(grid, x, y, currentColor, inColumnCount);
        }

        // 左对角线
        if (isLeftDiagonalUnbroken) {
            x = step.x + shift;
            y = step.y + shift;
            isLeftDiagonalUnbroken = count(grid, x, y, currentColor, inLeftDiagonalCount);
        }

        // 右对角线
        if (isRightDiagonalUnbroken) {
            x = step.x - shift;
            y = step.y - shift;
            isRightDiagonalUnbroken = count(grid, x, y, currentColor, inRightDiagonalCount);
        }
    }

    isLineUnbroken = true, isColumnUnbroken = true, isLeftDiagonalUnbroken = true, isRightDiagonalUnbroken = true;
    for (int shift = 1; shift <= 5; shift++) {
        // 行
        if (isLineUnbroken) {
            x = step.x;
            y = step.y + shift;
            isLineUnbroken = count(grid, x, y, currentColor, inLineCount);
        }

        // 列
        if (isColumnUnbroken) {
            x = step.x + shift;
            y = step.y;
            isColumnUnbroken = count(grid, x, y, currentColor, inColumnCount);
        }

        // 左对角线
        if (isLeftDiagonalUnbroken) {
            x = step.x + shift;
            y = step.y + shift;
            isLeftDiagonalUnbroken = count(grid, x, y, currentColor, inLeftDiagonalCount);
        }

        // 右对角线
        if (isRightDiagonalUnbroken) {
            x = step.x - shift;
            y = step.y - shift;
            isRightDiagonalUnbroken = count(grid, x, y, currentColor, inRightDiagonalCount);
        }
    }

    float totalScore = currentColor * (pow(inLineCount, 7) + pow(inColumnCount, 7) + pow(inLeftDiagonalCount, 7) +
                                       pow(inRightDiagonalCount, 7));

//    std::cout << step.x << ", " << step.y << ": " << inLineCount << " " << inColumnCount << " " << inLeftDiagonalCount << " " << inRightDiagonalCount << std::endl;

    return totalScore;
}