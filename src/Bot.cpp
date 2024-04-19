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

//    if (turnId >= 40) {
//        TOP_K = 10;
//    }

    return simulateStep(grid);
}


/**
 * 预推理，得到可选落点（top-k个）
 */
std::vector<Step> Bot::preSimulate(Grid& grid) {
    std::priority_queue<Step, std::vector<Step>, std::less<>> steps;
    std::vector<Step> availableSteps;

    for (Step step: grid.getAll()) {
        grid.doStep(step.x, step.y, botColor);
        long myScore = Evaluator(grid, botColor).preEvaluate(step);
        grid.doStep(step.x, step.y, -botColor);
        long enemyScore = Evaluator(grid, -botColor).preEvaluate(step);
        grid.undoStep(step.x, step.y);

        step.weight = myScore + enemyScore;

        steps.push(step);
    }

    for (int i=0; i<TOP_K && !steps.empty(); i++) {
        availableSteps.push_back(steps.top());
        steps.pop();
    }

    return availableSteps;
}


/**
 * 第一层推理，先进行预推理
 */
Move Bot::simulateStep(Grid& grid) {
    std::vector<Step> availableSteps = preSimulate(grid);

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
            childGrid.doStep(step0.x, step0.y, botColor);
            childGrid.doStep(step1.x, step1.y, botColor);

            //
            long childScore = simulateStep(childNode, childGrid, -botColor, 1);

            std::cout << "(" << step0.x << ", " << step0.y << "), (" << step1.x << ", " << step1.y << "): " << std::endl;
            childGrid.output();
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

//        parentGrid.doStep(parentNode->move.x0, parentNode->move.y0, botColor);
//        parentGrid.doStep(parentNode->move.x1, parentNode->move.y1, botColor);
//        long myScore = Evaluator(parentGrid, botColor).evaluate();
//
//        parentGrid.doStep(parentNode->move.x0, parentNode->move.y0, -botColor);
//        parentGrid.doStep(parentNode->move.x1, parentNode->move.y1, -botColor);
//        long enemyScore = Evaluator(parentGrid, -botColor).evaluate();
//
//        parentNode->score = myScore - enemyScore;

        parentNode->score = Evaluator(parentGrid, botColor).evaluate();

        return parentNode->score;
    }

    // 最大值，最小值
    long max = -LONG_MAX, min = LONG_MAX;

    std::vector<Step> availableSteps = preSimulate(parentGrid);

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
            currentGrid.doStep(step0.x, step0.y, currentColor);
            currentGrid.doStep(step1.x, step1.y, currentColor);

            // 局部检测，若已连成6子则无需继续推理
            long currentScore = Evaluator(currentGrid, currentColor).evaluate(currentMove);

            if (currentScore >= 100000000000) {
                // 已连成6子
//                std::cout << "already 6" << std::endl;

                parentNode->score = currentScore * 10;

                if (!parentNode->isMaxNode) {
                    parentNode->score *= -1;
                    parentNode->alpha = parentNode->score;
                } else {
                    parentNode->beta = parentNode->score;
                }

                return parentNode->score;

            } else {
                // 继续搜索
                currentScore = simulateStep(currentNode, currentGrid, -(currentColor), moveCount + 1);
            }

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
