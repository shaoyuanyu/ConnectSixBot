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
    Grid& grid;
    Color botColor;

    std::vector<long> scoreOfMyRoad = std::vector<long>(7);
    std::vector<long> scoreOfEnemyRoad = std::vector<long>(7);

    long calScore();
    long calScore(Move move);
    long calScore(Step step);
    inline int getCount(int x, int y);
    inline int revertCount(int count) const;

public:
    Evaluator(Grid& grid, Color botColor);

    void scan(Move move);
    void scan(Step step);

    long evaluate();
    long evaluate(Move move);

    long preEvaluate(Step step);
};


#endif //CONNECT_SIX_BOT_EVALUATOR_H
