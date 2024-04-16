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
 * 基于博弈树进行决策
 */
Turn Bot::makeDecision(Grid& grid, const int& turnId) {
    // 我方为黑开局，则转向开局库决策
    if (turnId == 1 && botColor == BLACK) return makeOpening();

    // 构建博弈树进行推理
//    auto* root = new GameNode(Turn(-1, -1, -1, -1), true);
//    simulateStep(root, grid, -botColor, 0);

    return simulateStep(grid);
}


/**
 * 预推理，得到可选落点（top-k个）
 */
void Bot::preSimulate(Grid& grid) {
    std::priority_queue<Step, std::vector<Step>, std::less<>> steps;

    Evaluator evaluatorOfMe, evaluatorOfEnemy;
    evaluatorOfMe.init(botColor, grid);
    evaluatorOfEnemy.init(-botColor, grid);

    for (Step step: grid.getAll()) {
        int weightOfMe = evaluatorOfMe.preEvaluate(step);
        int weightOfEnemy = evaluatorOfEnemy.preEvaluate(step);

        step.weight = (weightOfMe > weightOfEnemy) ? weightOfMe : weightOfEnemy;
        steps.push(step);
    }

    for (int i=0; i<TOP_K && !steps.empty(); i++) {
        availableSteps.push_back(steps.top());
        steps.pop();
    }
}


/**
 * 第一层推理，先进行预推理
 */
Turn Bot::simulateStep(Grid& grid) {
    preSimulate(grid);

    //
    Turn maxTurn = Turn(-1, -1, -1, -1);
    // 最大值，最小值
    float max = -FLT_MAX, min = FLT_MAX;
    //
    float alpha = FLT_MAX, beta = -FLT_MAX;

    for (int i=0; i<availableSteps.size(); i++) {
        Step step0 = availableSteps[i];
        for (int j = i + 1; j < availableSteps.size(); j++) {
            Step step1 = availableSteps[j];

            // 当前turn
            Turn childTurn(step0.x, step0.y, step1.x, step1.y);

            // childNode node
            auto* childNode = new GameNode(childTurn, false, alpha, beta);

            //
            float childScore = simulateStep(childNode, grid, botColor, 1);

            // delete childNode;

            if (childScore > max) {
                maxTurn = childTurn;
                max = childScore;
            }
            if (childScore > beta) beta = childScore;
        }
    }

    return maxTurn;
}


/**
 * 模拟走步
 * 递归建立博弈树，在叶节点调用评估函数，反向传播评估得分
 * minimax + alpha-beta剪枝
 */
float Bot::simulateStep(GameNode*& currentNode, Grid& currentGrid, const Color currentColor, int turnCount) {
    // 若搜索深度触底，终止搜索，进行评估
    if (turnCount == DEPTH_LIMIT) {
        currentNode->score = evaluator.evaluate(currentNode->turn);

        // 调试输出
//        std::cout << "(" << currentNode->turn.x0 << ", " << currentNode->turn.y0 << ")" << ", (" << currentNode->turn.x1 << ", " << currentNode->turn.y1 << ")" << std::endl;
//        std::cout << "turn: " << turnCount << std::endl;
//        std::cout << "score: " << currentNode->score << std::endl;
//        std::cout << std::endl;

        return currentNode->score;
    }

    // 执行上一手落子，获得用于当前节点的所有子节点的childGrid
    Grid childGrid = currentGrid;
    childGrid.doStep(currentNode->turn.x0, currentNode->turn.y0, currentColor);
    childGrid.doStep(currentNode->turn.x1, currentNode->turn.y1, currentColor);

    // 最大值，最小值
    float max = -FLT_MAX, min = FLT_MAX;

    //
    if (turnCount == DEPTH_LIMIT - 1) {
        evaluator.init(-currentColor, childGrid);
    }

    // 继续搜索
    for (int i=0; i<availableSteps.size(); i++) {
        Step step0 = availableSteps[i];
        if (currentGrid.data[step0.x][step0.y] != BLANK) continue;

        for (int j=i+1; j<availableSteps.size(); j++) {
            Step step1 = availableSteps[j];
            if (currentGrid.data[step1.x][step1.y] != BLANK) continue;

            // 当前turn
            Turn childTurn(step0.x, step0.y, step1.x, step1.y);

            // childNode node
            auto* childNode = new GameNode(childTurn, -(currentColor) != botColor, currentNode->alpha, currentNode->beta);

            // 继续搜索
            float childScore = simulateStep(childNode, childGrid, -(currentColor), turnCount + 1);

            // 释放内存
            delete childNode; // 耗时操作

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
