#include <iostream>
#include "Bot.h"

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
Move Bot::makeOpening() {
    return {7, 7, -1, -1};
}


/**
 * 基于博弈树进行决策
 */
Move Bot::makeDecision(Grid& grid, const int& turnId) {
    // 我方为黑开局，则转向开局库决策
    if (turnId == 1 && botColor == BLACK) return makeOpening();

    // 防止超时
    if (turnId >= 35) {
        DEPTH_LIMIT = 3;
        TOP_K = 8;
    }

    return simulateStep(grid);
}


/**
 * 预推理，得到可选落点（top-k个）
 */
void Bot::preSimulate(Grid grid) {
    std::priority_queue<Step, std::vector<Step>, std::less<>> steps;

    for (Step step: grid.getAll()) {
        //
        Grid experimentGrid = grid;
        Evaluator myEvaluator(experimentGrid, botColor);
        Evaluator enemyEvaluator(experimentGrid, -botColor);

        // 我方落子分数
        experimentGrid.doStep(step.x, step.y, botColor);
        long myScore = myEvaluator.preEvaluate(step);

        experimentGrid.undoStep(step.x, step.y);

        // 敌方落子分数
        experimentGrid.doStep(step.x, step.y, -botColor);
        long enemyScore = enemyEvaluator.preEvaluate(step);

        // 计算权重
        step.weight = myScore + enemyScore;

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
Move Bot::simulateStep(Grid& grid) {
    //
    preSimulate(grid);

    // 第一次move（我方）中最大者（相当于根max节点在进行选择）
    Move maxMove = Move(-1, -1, -1, -1);
    //
    long max = -LONG_MAX;
    long alpha = LONG_MAX, beta = -LONG_MAX;

    for (int i=0; i<availableSteps.size(); i++) {
        Step step0 = availableSteps[i];
        for (int j = i + 1; j < availableSteps.size(); j++) {
            Step step1 = availableSteps[j];

            //
            Move childMove(step0.x, step0.y, step1.x, step1.y);
            //
            auto* childNode = new GameNode(childMove, false, alpha, beta);
            //
            Grid childGrid = grid;
            childGrid.doMove(childMove, botColor);

            //
            long childScore = simulateStep(childNode, childGrid, -botColor, 1);

            //
            std::cout << "(" << step0.x << ", " << step0.y << "), (" << step1.x << ", " << step1.y << "): " << std::endl;
//            childGrid.output();
            std::cout << childScore << std::endl;
            std::cout << std::endl;

            if (childScore > max) {
                maxMove = childMove;
                max = childScore;
            }
            if (childScore > beta) beta = childScore;
        }
    }

    return maxMove;
}


/**
 * 模拟走步
 * 递归建立博弈树，在叶节点调用评估函数，反向传播评估得分
 * minimax + alpha-beta剪枝
 */
long Bot::simulateStep(GameNode*& parentNode, Grid& parentGrid, const Color currentColor, int moveCount) {
    // 若搜索深度触底，终止搜索，进行评估
    if (moveCount == DEPTH_LIMIT) {
        parentNode->score = Evaluator(parentGrid, botColor).evaluate();
        return parentNode->score;
    }

    // 熔断机制
    parentNode->score = Evaluator(parentGrid, botColor).evaluate(parentNode->move);
    if (parentNode->score >= 10000000000 || parentNode->score <= -10000000000) {
        parentNode->score = Evaluator(parentGrid, botColor).evaluate();
        return parentNode->score;
    }

    // 最大值，最小值
    long max = -LONG_MAX, min = LONG_MAX;

    // 继续搜索
    for (int i=0; i<availableSteps.size(); i++) {
        Step step0 = availableSteps[i];
        if (parentGrid.data[step0.y][step0.x] != BLANK) continue;

        for (int j=i+1; j<availableSteps.size(); j++) {
            Step step1 = availableSteps[j];
            if (parentGrid.data[step1.y][step1.x] != BLANK) continue;


            Move currentMove(step0.x, step0.y, step1.x, step1.y);
            //
            auto* currentNode = new GameNode(currentMove, currentColor != botColor, parentNode->alpha, parentNode->beta);
            //
            Grid currentGrid = parentGrid;
            currentGrid.doMove(currentMove, currentColor);

            long currentScore = simulateStep(currentNode, currentGrid, -(currentColor), moveCount + 1);

            // 释放内存
            delete currentNode; // 耗时操作

            // 分数反向传递 和 alpha-beta更新
            if (parentNode->isMaxNode) {
                if (currentScore > max) max = currentScore;
                if (currentScore > parentNode->beta) parentNode->beta = currentScore;
            } else {
                if (currentScore < min) min = currentScore;
                if (currentScore < parentNode->alpha) parentNode->alpha = currentScore;
            }

            // 检查alpha-beta，判断是否剪枝
            if (parentNode->alpha < parentNode->beta) {
                parentNode->score = (parentNode->isMaxNode) ? max : min;
                return parentNode->score;
            }
        }
    }

    parentNode->score = (parentNode->isMaxNode) ? max : min;

    return parentNode->score;
}
