//
// 评估模块
//

#ifndef CONNECT_SIX_BOT_EVALUATOR_H
#define CONNECT_SIX_BOT_EVALUATOR_H


#include <vector>
#include <climits>
#include <cmath>
#include "GameConfig.h"
#include "Grid.h"

class Evaluator {
private:
    Grid& grid;
    Color botColor;

    std::vector<long> scoreOfMyRoad = std::vector<long>(7);
    std::vector<long> scoreOfEnemyRoad = std::vector<long>(7);

    inline void updateRoadCount(int maxRoad, std::vector<int>& countOfMyRoad, std::vector<int>& countOfEnemyRoad);

public:
    Evaluator(Grid& grid, Color botColor);

    long evaluate();
    long evaluate(Move move);
    long preEvaluate(Step step);
};


#endif //CONNECT_SIX_BOT_EVALUATOR_H
