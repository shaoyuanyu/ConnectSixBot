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
Move Bot::makeDecision(Grid& grid, const int& moveId) {
    // 我方为黑开局，则转向开局库决策
    if (moveId == 1 && botColor == BLACK) return makeOpening();

    // 构建博弈树进行推理
    return simulateStep(grid);
}


/**
 * 预推理，得到可选落点（top-k个）
 */
void Bot::preSimulate(Grid& grid) {
    std::priority_queue<Step, std::vector<Step>, std::less<>> steps;

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
}


/**
 * 第一层推理，先进行预推理
 */
Move Bot::simulateStep(Grid& grid) {
    preSimulate(grid);

    //
    Move maxMove = Move(-1, -1, -1, -1);
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
            long childScore = simulateStep(childNode, childGrid, botColor, 1);

            // delete childNode;

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
long Bot::simulateStep(GameNode*& currentNode, Grid& currentGrid, const Color currentColor, int moveCount) {
    // 若搜索深度触底，终止搜索，进行评估
    if (moveCount == DEPTH_LIMIT) {
        long myScore = Evaluator(currentGrid, currentColor).evaluate();

        currentGrid.doStep(currentNode->move.x0, currentNode->move.y0, -currentColor);
        currentGrid.doStep(currentNode->move.x1, currentNode->move.y1, -currentColor);

        long enemyScore = Evaluator(currentGrid, -currentColor).evaluate();

        currentNode->score = myScore + enemyScore;

        return currentNode->score;
    }

    // 最大值，最小值
    long max = -LONG_MAX, min = LONG_MAX;

    // 继续搜索
    for (int i=0; i<availableSteps.size(); i++) {
        Step step0 = availableSteps[i];
        if (currentGrid.data[step0.x][step0.y] != BLANK) continue;

        for (int j=i+1; j<availableSteps.size(); j++) {
            Step step1 = availableSteps[j];
            if (currentGrid.data[step1.x][step1.y] != BLANK) continue;


            Move childMove(step0.x, step0.y, step1.x, step1.y);
            //
            auto* childNode = new GameNode(childMove, -(currentColor) != botColor, currentNode->alpha, currentNode->beta);
            //
            Grid childGrid = currentGrid;
            childGrid.doStep(step0.x, step0.y, currentColor);
            childGrid.doStep(step1.x, step1.y, currentColor);

            // 继续搜索
            long childScore = simulateStep(childNode, childGrid, -(currentColor), moveCount + 1);

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
