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
    Color currentColor;
    Grid grid;

    const int ALERT_SCORE = 20;
    int baseScore = INT_MIN;

    std::vector<std::vector<int>> scoreOfMyRoad = std::vector<std::vector<int>>(2, std::vector<int>(10));
    std::vector<std::vector<int>> scoreOfEnemyRoad = std::vector<std::vector<int>>(2, std::vector<int>(10));

    int calScore();
    int calScore(Turn move);
    inline int getCount(int x, int y);
    inline void updateRoadTypeNum(int count, std::vector<int>& countOfMyRoad, std::vector<int>& countOfEnemyRoad) const;

public:
    Evaluator(Color botColor, Grid grid);
    int evaluate(Turn move);
};


#endif //CONNECT_SIX_BOT_EVALUATOR_H
