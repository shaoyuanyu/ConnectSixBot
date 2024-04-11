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


Color Bot::getColor() const {
    return botColor;
}


Color Bot::getOppositeColor() const {
    return -botColor;
}


/**
 * 基于开局库进行开局决策
 */
Turn Bot::makeOpening() {
    return Turn(7, 7, -1, -1);
}


/**
 * 根据点位权重计算深度，主要是为了设置深度上限
 */
inline int Bot::getDepthByWeight(int weight0, int weight1) {
//    int averageWeight = (weight0 + weight1) / 2;
    int averageWeight = (weight0 < weight1) ? weight0 : weight1;

    if (averageWeight <= 2) return 0;
    else if (averageWeight <= 6) return 1;
    else if (averageWeight <= 12) return 2;
    else if (averageWeight <= 15) return 3;

    return 2;
}


/**
 * 基于博弈树进行决策
 */
Turn Bot::makeDecision(Grid& grid, const int& turnId) {
    // 我方为黑开局，则转向开局库决策
    if (turnId == 1 && botColor == BLACK) return makeOpening();

    // 获取可选落子点
    std::vector<Step> availableSteps = grid.getAvailable(25);

    // 调试输出
    for (Step availableStep: availableSteps) {
        std::cout << "(" << availableStep.x << ", " << availableStep.y << ")\t";
    }
    std::cout << std::endl << std::endl;

    float max = -FLT_MAX;
    Turn maxTurn(-1, -1, -1, -1);

    for (int i=0; i<availableSteps.size(); i++) {
        Step step0 = availableSteps[i];
        for (int j=i+1; j<availableSteps.size(); j++) {
            Step step1 = availableSteps[j];

            // turn
            Turn thisTurn = Turn(step0.x, step0.y, step1.x, step1.y);
            //
            auto* child = new GameNode(thisTurn, false);
            firstTurnNodes.push_back(child);
            // grid
            Grid nextGrid = grid;
            nextGrid.doStep(step0.x, step0.y, botColor);
            nextGrid.doStep(step1.x, step1.y, botColor);
            // pre turns
            std::vector<Turn> preTurns;
            preTurns.push_back(thisTurn);
            // depth limit
            int currentDepthLimit = basicDepthLimit + getDepthByWeight(grid.weight[step0.x][step0.y], grid.weight[step1.x][step1.y]);

            // 模拟落子构建博弈树
            float score = simulateStep(child, nextGrid, preTurns, botColor, 1, currentDepthLimit);

            // 调试输出
            std::cout << "(" << child->turn.x0 << ", " << child->turn.y0 << ")" << ", (" << child->turn.x1 << ", " << child->turn.y1 << ")" << std::endl;
            std::cout << "score: " << score << std::endl;
            std::cout << "depth: " << currentDepthLimit << std::endl;
            std::cout << std::endl;

            // 更新最大分数和落子
            if (score > max) {
                max = score;
                maxTurn = thisTurn;
            }
        }
    }

    return maxTurn;
}


/**
 * 模拟走步
 * 递归建立博弈树，在叶节点调用评估函数，反向传播评估得分
 * minimax + alpha-beta剪枝
 */
float Bot::simulateStep(GameNode*& currentNode, Grid& currentGrid, const std::vector<Turn>& preTurns, const Color currentColor, int turnCount, int currentDepthLimit) {
    // 若搜索深度触底，终止搜索，进行评估
    if (turnCount == currentDepthLimit) {
        for (Turn preTurn: preTurns) {
            currentNode->score += evaluate(currentGrid, Step(preTurn.x0, preTurn.y0));
            currentNode->score += evaluate(currentGrid, Step(preTurn.x1, preTurn.y1));
        }

        return currentNode->score;
    }

    // 最大值，最小值
    float max = -FLT_MAX, min = FLT_MAX;

    // 继续搜索
    std::vector<Step> availableSteps = currentGrid.getAvailable(25/turnCount);

    for (int i=0; i<availableSteps.size(); i++) {
        Step step0 = availableSteps[i];
        for (int j=i+1; j<availableSteps.size(); j++) {
            Step step1 = availableSteps[j];

            // this turn
            Turn currentTurn(step0.x, step0.y, step1.x, step1.y);
            // child node
            auto* child = new GameNode(currentTurn, -(currentColor) != botColor, currentNode->alpha, currentNode->beta);
            // next grid
            Grid nextGrid = currentGrid;
            nextGrid.doStep(step0.x, step0.y, currentColor);
            nextGrid.doStep(step1.x, step1.y, currentColor);
            // new preTurns list
            std::vector<Turn> newTurns = preTurns;
            newTurns.push_back(currentTurn);

            // 继续搜索
            float childScore = simulateStep(child, nextGrid, newTurns, -(currentColor), turnCount + 1, currentDepthLimit);

            // 释放内存
            delete child; // 耗时操作

            // 分数反向传递 和 alpha-beta更新
            if (currentNode->isMaxNode) {
                if (childScore > max) max = childScore;
                if (childScore > currentNode->beta) currentNode->beta = childScore;
            } else {
                if (childScore < min) min = childScore;
                if (childScore < currentNode->alpha) currentNode->alpha = childScore;
            }

            // 检查alpha-beta，判断是否剪枝
            if (currentNode->alpha < currentNode->beta) {
                currentNode->score = (currentNode->isMaxNode) ? max : min;
                return currentNode->score;
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

    if (grid.data[x][y] == currentColor) {
        // 同色
        count += 1;
    } else if (grid.data[x][y] == -currentColor) {
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
    Color currentColor = grid.data[step.x][step.y];
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