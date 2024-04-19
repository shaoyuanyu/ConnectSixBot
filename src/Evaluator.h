//
// 评估模块
//

#ifndef CONNECT_SIX_BOT_EVALUATOR_H
#define CONNECT_SIX_BOT_EVALUATOR_H


#include <vector>
#include <climits>
#include "GameConfig.h"
#include "Grid.h"

class Evaluator {
private:
    Grid grid;
    Color currentColor;

    std::vector<long> scoreOfMyRoad = std::vector<long>(7);
    std::vector<long> scoreOfEnemyRoad = std::vector<long>(7);

    long calScore();
    long calScore(Move move);
    inline int getCount(int x, int y);
    inline void updateRoadTypeNum(int count, std::vector<int>& countOfMyRoad, std::vector<int>& countOfEnemyRoad) const;

public:
    Evaluator(Grid grid, Color currentColor);
    long evaluate();
    long evaluate(Move move);
    long preEvaluate(Step step);
};


#endif //CONNECT_SIX_BOT_EVALUATOR_H
