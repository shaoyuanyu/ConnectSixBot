#include "Bot.h"
#include "Evaluator.h"
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
 * 基于博弈树进行决策
 */
Turn Bot::makeDecision(Grid& grid, const int& turnId) {
    // 我方为黑开局，则转向开局库决策
    if (turnId == 1 && botColor == BLACK) return makeOpening();

    // 构建博弈树进行推理
    auto* root = new GameNode(Turn(-1, -1, -1, -1), true);
    simulateStep(root, grid, std::vector<Turn>(), -botColor, 0);

    return maxTurn;
}


/**
 * 模拟走步
 * 递归建立博弈树，在叶节点调用评估函数，反向传播评估得分
 * minimax + alpha-beta剪枝
 */
float Bot::simulateStep(GameNode*& currentNode, Grid& currentGrid, const std::vector<Turn>& preTurns, const Color currentColor, int turnCount) {
    // 若搜索深度触底，终止搜索，进行评估
    if (turnCount == depthLimit) {
        for (Turn preTurn: preTurns) {
            currentNode->score += evaluate(currentGrid, preTurn, currentColor);
        }

        // 调试输出
        std::cout << "(" << currentNode->turn.x0 << ", " << currentNode->turn.y0 << ")" << ", (" << currentNode->turn.x1 << ", " << currentNode->turn.y1 << ")" << std::endl;
        std::cout << "turn: " << turnCount << std::endl;
        std::cout << "score: " << currentNode->score << std::endl;
        std::cout << std::endl;

        return currentNode->score;
    }

    // 最大值，最小值
    float max = -FLT_MAX, min = FLT_MAX;

    // 继续搜索
    std::vector<Step> availableSteps = currentGrid.getAvailable();

    for (int i=0; i<availableSteps.size(); i++) {
        Step step0 = availableSteps[i];
        for (int j=i+1; j<availableSteps.size(); j++) {
            Step step1 = availableSteps[j];

            // this turn
            Turn childTurn(step0.x, step0.y, step1.x, step1.y);
            // child node
            auto* child = new GameNode(childTurn, -(currentColor) != botColor, currentNode->alpha, currentNode->beta);
            // next grid
            Grid childGrid = currentGrid;
            childGrid.doStep(step0.x, step0.y, currentColor);
            childGrid.doStep(step1.x, step1.y, currentColor);
            // new preTurns list
            std::vector<Turn> childPreTurns = preTurns;
            childPreTurns.push_back(childTurn);

            // 继续搜索
            float childScore = simulateStep(child, childGrid, childPreTurns, -(currentColor), turnCount + 1);

            // 释放内存
            delete child; // 耗时操作

            // 分数反向传递 和 alpha-beta更新
            if (currentNode->isMaxNode) {
                if (childScore > max) {
                    max = childScore;
                    if (turnCount == 0) maxTurn = childTurn;
                }
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
 * 评估函数
 */
float Bot::evaluate(Grid& grid, Turn move, Color currentColor) {
    return Evaluator(currentColor, grid).evaluate(move);
}