//
// 六子棋
//

#include <iostream>
#include <sys/time.h>
#include <vector>
#include <cfloat>
#include <climits>
#include <queue>


// config
/**
 * 棋盘
 */
#define GRID_SIZE 15

/**
 * 棋子颜色
 */
#define WHITE (-1)
#define BLANK 0
#define BLACK 1
typedef int Color;

/**
 * 一手落子
 */
struct Step {
    int x;
    int y;

    int weight = 0;

    Step(int x, int y): x(x), y(y) {}

    bool operator<(const Step& other) const {
        return weight < other.weight;
    }
};

/**
 * 一回合包括两手落子
 */
struct Turn {
    int x0, y0;
    int x1, y1;

    Turn(int x0, int y0, int x1, int y1): x0(x0), y0(y0), x1(x1), y1(y1) {}
    Turn(Step step0, Step step1): x0(step0.x), y0(step0.y), x1(step1.x), y1(step1.y) {}
};


// grid
struct Grid {
    Color data[GRID_SIZE][GRID_SIZE] = { BLANK };

    void doStep(int x, int y, Color color);
    void undoStep(int x, int y);
    std::vector<Step> getAll();
    std::vector<Step> getAvailable();
    void output();
};


void Grid::doStep(int x, int y, Color color) {
    if (x == -1) return;

    if (data[x][y] == BLANK) {
        data[x][y] = color;
    }
}


void Grid::undoStep(int x, int y) {
    data[x][y] = BLANK;
}


std::vector<Step> Grid::getAll() {
    std::vector<Step> availableSteps;

    for (int x=0; x<GRID_SIZE; x++) {
        for (int y=0; y<GRID_SIZE; y++) {
            if (data[x][y] != BLANK) continue;
            availableSteps.emplace_back(x, y);
        }
    }

    return availableSteps;
}


// game node
struct GameNode {
    float score = 0.0;
    Turn turn; // 一回合，两手落子
    bool isMaxNode; // 是否为Max节点
    float alpha = FLT_MAX; // 上确界
    float beta = -FLT_MAX; // 下确界

    GameNode(Turn turn, bool isMaxNode): turn(turn), isMaxNode(isMaxNode) {}
    GameNode(Turn turn, bool isMaxNode, float alpha, float beta): turn(turn), isMaxNode(isMaxNode), alpha(alpha), beta(beta) {}

    bool operator<(const GameNode& other) const;
};


// evaluator
class Evaluator {
private:
    Color currentColor;
    Grid grid;
    const int ALERT_SCORE = 9;
    int baseScore = INT_MIN;

    std::vector<std::vector<int>> scoreOfMyRoad = std::vector<std::vector<int>>(2, std::vector<int>(10));
    std::vector<std::vector<int>> scoreOfEnemyRoad = std::vector<std::vector<int>>(2, std::vector<int>(10));

    int calScore();
    int calScore(Turn move);
    inline int getCount(int x, int y);
    inline void updateRoadTypeNum(int count, std::vector<int>& countOfMyRoad, std::vector<int>& countOfEnemyRoad) const;

public:
    Evaluator();
    void init(Color currentColor, Grid grid);
    int evaluate(Turn move);
    int preEvaluate(Step step);
};


/**
 * Evaluator构造方法
 */
Evaluator::Evaluator() {
    //防守型
    scoreOfMyRoad[0][1] = 1;
    scoreOfMyRoad[0][2] = 5;
    scoreOfMyRoad[0][3] = 10;
    scoreOfMyRoad[0][4] = 25;
    scoreOfMyRoad[0][5] = 25;
    scoreOfMyRoad[0][6] = 10000;
    scoreOfEnemyRoad[0][1] = 1;
    scoreOfEnemyRoad[0][2] = 5;
    scoreOfEnemyRoad[0][3] = 15;
    scoreOfEnemyRoad[0][4] = 35;
    scoreOfEnemyRoad[0][5] = 25;
    scoreOfEnemyRoad[0][6] = 10000;

    //进攻型
    scoreOfMyRoad[1][1] = 1;
    scoreOfMyRoad[1][2] = 15;
    scoreOfMyRoad[1][3] = 30;
    scoreOfMyRoad[1][4] = 50;
    scoreOfMyRoad[1][5] = 50;
    scoreOfMyRoad[1][6] = 10000;
    scoreOfEnemyRoad[1][1] = 1;
    scoreOfEnemyRoad[1][2] = 10;
    scoreOfEnemyRoad[1][3] = 15;
    scoreOfEnemyRoad[1][4] = 35;
    scoreOfEnemyRoad[1][5] = 25;
    scoreOfEnemyRoad[1][6] = 10000;
}


/**
 *
 */
void Evaluator::init(Color givenColor, Grid givenGrid) {
    //
    currentColor = givenColor;
    grid = givenGrid;

    // 全局扫描并计算baseScore
    baseScore = calScore();
}


/**
 * 所下棋子的价值：下棋后局面分数 - 下棋前局面分数
 */
int Evaluator::evaluate(Turn move) {
    int fScore = calScore(move);

    grid.doStep(move.x0, move.y0, currentColor);
    grid.doStep(move.x1, move.y1, currentColor);

    int bScore = calScore(move);

    grid.undoStep(move.x0, move.y0);
    grid.undoStep(move.x1, move.y1);

    return bScore - fScore;
}


/**
 * 计数
 * 黑棋加一分，白棋加7分
 */
inline int Evaluator::getCount(int x, int y) {
    return (grid.data[x][y] == WHITE) ? 7 : grid.data[x][y];
}


/**
 * 更新不同路类型的数量
 */
inline void Evaluator::updateRoadTypeNum(int count, std::vector<int>& countOfMyRoad, std::vector<int>& countOfEnemyRoad) const {
    // 没棋 或 黑白混杂
    if (count == 0 || (count > 6 && count % 7 != 0)) return;

    if (count < 7) {
        if (currentColor == BLACK) {
            countOfMyRoad[count]++;
        } else {
            countOfEnemyRoad[count]++;
        }
    } else {
        if (currentColor == BLACK) {
            countOfEnemyRoad[count/7]++;
        } else {
            countOfMyRoad[count/7]++;
        }
    }
}


/**
 * 计算此时局面的总分数，得到baseScore(全局扫描)
 */
int Evaluator::calScore() {
    int score = 0;
    int condition = 0; // condition用于表示进攻型还是防守型, 1进攻, 0防守
    std::vector<int> countOfMyRoad = std::vector<int>(10, 0);
    std::vector<int> countOfEnemyRoad = std::vector<int>(10, 0);

    // 纵向扫描
    for (int x=0; x<GRID_SIZE-5; x++) {
        for (int y=0; y<GRID_SIZE; y++) {
            int count = getCount(x, y) + getCount(x + 1, y) + getCount(x + 2, y) + getCount(x + 3, y) + getCount(x + 4, y) + getCount(x + 5, y);
            updateRoadTypeNum(count, countOfMyRoad, countOfEnemyRoad);
        }
    }
    // 横向扫描
    for (int x=0; x<GRID_SIZE; x++) {
        for (int y=0; y<GRID_SIZE-5; y++) {
            int count = getCount(x, y) + getCount(x, y + 1) + getCount(x, y + 2) + getCount(x, y + 3) + getCount(x, y + 4) + getCount(x, y + 5);
            updateRoadTypeNum(count, countOfMyRoad, countOfEnemyRoad);
        }
    }
    // 右下对角扫描
    for (int x=0; x<GRID_SIZE-5; x++) {
        for (int y=0; y<GRID_SIZE-5; y++) {
            int count = getCount(x, y) + getCount(x + 1, y + 1) + getCount(x + 2, y + 2) + getCount(x + 3, y + 3) + getCount(x + 4, y + 4) + getCount(x + 5, y + 5);
            updateRoadTypeNum(count, countOfMyRoad, countOfEnemyRoad);
        }
    }
    // 左下对角扫描
    for (int x=0; x<GRID_SIZE-5; x++) {
        for (int y=5; y<GRID_SIZE; y++) {
            int count = getCount(x, y) + getCount(x + 1, y - 1) + getCount(x + 2, y - 2) + getCount(x + 3, y - 3) + getCount(x + 4, y - 4) + getCount(x + 5, y - 5);
            updateRoadTypeNum(count, countOfMyRoad, countOfEnemyRoad);
        }
    }

    //
    for (int i=1; i<=6; i++) {
        score += (countOfMyRoad[i] * scoreOfMyRoad[condition][i] - countOfEnemyRoad[i] * scoreOfEnemyRoad[condition][i]);
    }

    return score;
}


/**
 * 计算此时局面的总分数(局部扫描)
 */
int Evaluator::calScore(Turn move) {
    int score = 0;
    int condition = 0; // condition用于表示进攻型还是防守型, 1进攻, 0防守
    int x0 = move.x0, y0 = move.y0, x1 = move.x1, y1 = move.y1;
    std::vector<int> countOfMyRoad = std::vector<int>(10, 0);
    std::vector<int> countOfEnemyRoad = std::vector<int>(10, 0);

    if (baseScore > ALERT_SCORE) {
        condition = 1;
    }

    // 纵向扫描
    for (int x=x0-5, y=y0; x<GRID_SIZE-5 && x<=x0; x++) {
        if (x < 0) continue;

        int count = getCount(x, y) + getCount(x + 1, y) + getCount(x + 2, y) + getCount(x + 3, y) + getCount(x + 4, y) + getCount(x + 5, y);
        updateRoadTypeNum(count, countOfMyRoad, countOfEnemyRoad);
    }
    for (int x=x1-5, y=y1; x<GRID_SIZE-5 && x<=x1; x++) {
        if (x < 0) continue;
        if (x == x0 || x + 1 == x0 || x + 2 == x0 || x + 3 == x0 || x + 4 == x0 || x + 5 == x0) continue; // 跳过重复扫描

        int count = getCount(x, y) + getCount(x + 1, y) + getCount(x + 2, y) + getCount(x + 3, y) + getCount(x + 4, y) + getCount(x + 5, y);
        updateRoadTypeNum(count, countOfMyRoad, countOfEnemyRoad);
    }
    // 横向扫描
    for (int x=x0, y=y0-5; y<GRID_SIZE-5 && y<=y0; y++) {
        if (y < 0) continue;

        int count = getCount(x, y) + getCount(x, y + 1) + getCount(x, y + 2) + getCount(x, y + 3) + getCount(x, y + 4) + getCount(x, y + 5);
        updateRoadTypeNum(count, countOfMyRoad, countOfEnemyRoad);
    }
    for (int x=x1, y=y1-5; y<GRID_SIZE-5 && y<=y1; y++) {
        if (y < 0) continue;
        if (y == y0 || y + 1 == y0 || y + 2 == y0 || y + 3 == y0 || y + 4 == y0 || y + 5 == y0) continue; // 跳过重复扫描

        int count = getCount(x, y) + getCount(x, y + 1) + getCount(x, y + 2) + getCount(x, y + 3) + getCount(x, y + 4) + getCount(x, y + 5);
        updateRoadTypeNum(count, countOfMyRoad, countOfEnemyRoad);
    }
    // 右下对角扫描
    for (int x=x0-5, y=y0-5; (x<GRID_SIZE-5 && x<=x0) && (y<GRID_SIZE-5 && y<=y0); x++, y++) {
        if (x < 0 || y < 0) continue;

        int count = getCount(x, y) + getCount(x + 1, y + 1) + + getCount(x + 2, y + 2) + + getCount(x + 3, y + 3) + + getCount(x + 4, y + 4) + + getCount(x + 5, y + 5);
        updateRoadTypeNum(count, countOfMyRoad, countOfEnemyRoad);
    }
    for (int x=x1-5, y=y1-5; (x<GRID_SIZE-5 && x<=x1) && (y<GRID_SIZE-5 && y<=y1); x++, y++) {
        if (x < 0 || y < 0) continue;
        if ((x == x0 && y == y0) || (x + 1 == x0 && y + 1 == y0) || (x + 2 == x0 && y + 2 == y0)|| (x + 3 == x0 && y + 3 == y0) || (x + 4 == x0 && y + 4 == y0) || (x + 5 == x0 && y + 5 == y0)) continue; // 跳过重复扫描

        int count = getCount(x, y) + getCount(x + 1, y + 1) + + getCount(x + 2, y + 2) + + getCount(x + 3, y + 3) + + getCount(x + 4, y + 4) + + getCount(x + 5, y + 5);
        updateRoadTypeNum(count, countOfMyRoad, countOfEnemyRoad);
    }
    // 左下对角扫描
    for (int x=x0-5, y=y0+5; (x<GRID_SIZE-5 && x<=x0) && (y>=y0); x++, y--) {
        if (x < 0 || y > GRID_SIZE) continue;

        int count = getCount(x, y) + getCount(x + 1, y - 1) + + getCount(x + 2, y - 2) + + getCount(x + 3, y - 3) + + getCount(x + 4, y - 4) + + getCount(x + 5, y - 5);
        updateRoadTypeNum(count, countOfMyRoad, countOfEnemyRoad);
    }
    for (int x=x1-5, y=y1+5; (x<GRID_SIZE-5 && x<=x1) && (y>=y1); x++, y--) {
        if (x < 0 || y > GRID_SIZE) continue;
        if ((x == x0 && y == y0) || (x + 1 == x0 && y - 1 == y0) || (x + 2 == x0 && y - 2 == y0)|| (x + 3 == x0 && y - 3 == y0) || (x + 4 == x0 && y - 4 == y0) || (x + 5 == x0 && y - 5 == y0)) continue; // 跳过重复扫描

        int count = getCount(x, y) + getCount(x + 1, y - 1) + + getCount(x + 2, y - 2) + + getCount(x + 3, y - 3) + + getCount(x + 4, y - 4) + + getCount(x + 5, y - 5);
        updateRoadTypeNum(count, countOfMyRoad, countOfEnemyRoad);
    }

    //
    for(int i = 1; i < 7; i++) {
        score += (countOfMyRoad[i] * scoreOfMyRoad[condition][i] - countOfEnemyRoad[i] * scoreOfEnemyRoad[condition][i]);
    }

    return score;
}


/**
 * 预评估
 */
int Evaluator::preEvaluate(Step step) {
    int score = 0;
    int condition = 0; // condition用于表示进攻型还是防守型, 1进攻, 0防守
    std::vector<int> countOfMyRoad = std::vector<int>(10, 0);
    std::vector<int> countOfEnemyRoad = std::vector<int>(10, 0);

    if (baseScore > ALERT_SCORE) {
        condition = 1;
    }

    // 纵向扫描
    for (int x=step.x-5, y=step.y; x<GRID_SIZE-6 && x<=step.x; x++) {
        if (x < 0) continue;

        int count = getCount(x, y) + getCount(x + 1, y) + getCount(x + 2, y) + getCount(x + 3, y) + getCount(x + 4, y) + getCount(x + 5, y);
        updateRoadTypeNum(count, countOfMyRoad, countOfEnemyRoad);
    }
    // 横向扫描
    for (int x=step.x, y=step.y-5; y<GRID_SIZE-5 && y<=step.y; y++) {
        if (y < 0) continue;

        int count = getCount(x, y) + getCount(x, y + 1) + getCount(x, y + 2) + getCount(x, y + 3) + getCount(x, y + 4) + getCount(x, y + 5);
        updateRoadTypeNum(count, countOfMyRoad, countOfEnemyRoad);
    }
    // 右下对角扫描
    for (int x=step.x-5, y=step.y-5; (x<GRID_SIZE-5 && x<=step.x) && (y<GRID_SIZE-5 && y<=step.y); x++, y++) {
        if (x < 0 || y < 0) continue;

        int count = getCount(x, y) + getCount(x + 1, y + 1) + + getCount(x + 2, y + 2) + + getCount(x + 3, y + 3) + + getCount(x + 4, y + 4) + + getCount(x + 5, y + 5);
        updateRoadTypeNum(count, countOfMyRoad, countOfEnemyRoad);
    }
    // 左下对角扫描
    for (int x=step.x-5, y=step.y+5; (x<GRID_SIZE-5 && x<=step.x) && (y>=step.y); x++, y--) {
        if (x < 0 || y > GRID_SIZE) continue;

        int count = getCount(x, y) + getCount(x + 1, y - 1) + + getCount(x + 2, y - 2) + + getCount(x + 3, y - 3) + + getCount(x + 4, y - 4) + + getCount(x + 5, y - 5);
        updateRoadTypeNum(count, countOfMyRoad, countOfEnemyRoad);
    }

    for(int i = 1; i < 7; i++) {
        score += (countOfMyRoad[i] * scoreOfMyRoad[condition][i] - countOfEnemyRoad[i] * scoreOfEnemyRoad[condition][i]);
    }

    return score;
}


// bot
class Bot {
private:
    Color botColor;
    const int DEPTH_LIMIT = 3; // 每次推理深度
    const int TOP_K = 20;

    Evaluator evaluator;

    std::vector<GameNode*> firstTurnNodes;

    std::vector<Step> availableSteps;

    Turn makeOpening();
    Turn simulateStep(Grid& grid);
    void preSimulate(Grid& grid);
    float simulateStep(GameNode*& currentRoot, Grid& currentGrid, Color currentColor, int turnCount);

public:
    void setColor(Color c);
    Color getColor() const; // 获取我方颜色
    Color getOppositeColor() const; // 获取对手颜色
    Turn makeDecision(Grid& grid, const int& turnId); // 基于博弈树进行决策
};


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


//
int inputGrid(Grid& grid, Bot& bot) {
    int x0, y0, x1, y1;
    int turnId;

    std::cin >> turnId;

    // 假设我方执白
    bot.setColor(WHITE);

    for (int i=0; i<turnId; i++) {
        std::cin >> x0 >> y0 >> x1 >> y1;

        if (x0 == -1) {
            // 我方执黑
            bot.setColor(BLACK);
        }

        // 对手落子
        grid.doStep(x0, y0, bot.getOppositeColor());
        grid.doStep(x1, y1, bot.getOppositeColor());

        // 我方落子
        if (i < turnId - 1) {
//            std::cout << "my step: ";
            std::cin >> x0 >> y0 >> x1 >> y1;

            grid.doStep(x0, y0, bot.getColor());
            grid.doStep(x1, y1, bot.getColor());
        }
    }

    return turnId;
}

int main() {
    std::ios::sync_with_stdio(false);

    Grid grid;
    Bot bot;

    int turnId = inputGrid(grid, bot);

    Turn result = bot.makeDecision(grid, turnId);

    std::cout << result.x0 << ' ' << result.y0 << ' ' << result.x1 << ' ' << result.y1 << std::endl;

    return 0;
}