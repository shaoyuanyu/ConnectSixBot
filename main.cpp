#include <iostream>
#include <cstdlib>
#include <ctime>

#define GRIDSIZE 15
#define judge_black 0
#define judge_white 1
#define grid_blank 0
#define grid_black 1
#define grid_white -1

int currBotColor; // 本方所执子颜色（1为黑，-1为白，棋盘状态亦同）
int gridInfo[GRIDSIZE][GRIDSIZE] = { 0 }; // 先x后y，记录棋盘状态
int dx[] = { -1,-1,-1,0,0,1,1,1 };
int dy[] = { -1,0,1,-1,1,-1,0,1 };

// 判断是否在棋盘内
inline bool inMap(int x, int y) {
    if (x < 0 || x >= GRIDSIZE || y < 0 || y >= GRIDSIZE)
        return false;
    return true;
}


// 在坐标处落子，检查是否合法或模拟落子
bool ProcStep(int x0, int y0, int x1, int y1, int grid_color, bool check_only) {
    if (x1 == -1 || y1 == -1) {
        if (!inMap(x0, y0) || gridInfo[x0][y0] != grid_blank)
            return false;
        if (!check_only) {
            gridInfo[x0][y0] = grid_color;
        }
        return true;
    }
    else {
        if ((!inMap(x0, y0)) || (!inMap(x1, y1)))
            return false;
        if (gridInfo[x0][y0] != grid_blank || gridInfo[x1][y1] != grid_blank)
            return false;
        if (!check_only) {
            gridInfo[x0][y0] = grid_color;
            gridInfo[x1][y1] = grid_color;
        }
        return true;
    }
}

int main() {
    int x0, y0, x1, y1;

    // 分析自己收到的输入和自己过往的输出，并恢复棋盘状态
    int turnID;
    std::cin >> turnID;
    currBotColor = grid_white; // 先假设自己是白方
    for (int i = 0; i < turnID; i++) {
        // 根据这些输入输出逐渐恢复状态到当前回合
        std::cin >> x0 >> y0 >> x1 >> y1;
        if (x0 == -1)
            currBotColor = grid_black; // 第一回合收到坐标是-1, -1，说明我是黑方
        if (x0 >= 0)
            ProcStep(x0, y0, x1, y1, -currBotColor, false); // 模拟对方落子
        if (i < turnID - 1) {
            std::cin >> x0 >> y0 >> x1 >> y1;
            if (x0 >= 0)
                ProcStep(x0, y0, x1, y1, currBotColor, false); // 模拟己方落子
        }
    }

    /************************************************************************************/
    /***在下面填充你的代码，决策结果（本方将落子的位置）存入startX、startY、resultX、resultY中*****/
    //下面仅为随机策略的示例代码，且效率低，可删除
    int startX, startY, resultX, resultY;
    int beginPos[GRIDSIZE*GRIDSIZE][2], possiblePos[GRIDSIZE*GRIDSIZE][2];
    bool selfFirstBlack = (turnID == 1 && currBotColor == grid_black);//本方是黑方先手

    int posCount = 0, choice0, choice1;
    for (int i = 0; i < GRIDSIZE; ++i) {
        for (int j = 0; j < GRIDSIZE; ++j) {
            if (gridInfo[i][j] == grid_blank) {
                beginPos[posCount][0] = i;
                beginPos[posCount][1] = j;
                possiblePos[posCount][0] = i;
                possiblePos[posCount++][1] = j;
            }
        }
    }

    // 做出决策
    if (posCount > 0) {
        srand(time(0));
        choice0 = rand() % posCount;
        startX = beginPos[choice0][0];
        startY = beginPos[choice0][1];
        choice1 = rand() % posCount;
        if (selfFirstBlack) {
            //黑方先手只下一子
            resultX = -1;
            resultY = -1;
        } else {
            while (choice0 == choice1) {
                choice1 = rand() % posCount;
            }
            resultX = possiblePos[choice1][0];
            resultY = possiblePos[choice1][1];
        }
    } else {
        startX = -1;
        startY = -1;
        resultX = -1;
        resultY = -1;
    }
    /****在上方填充你的代码，决策结果（本方将落子的位置）存入startX、startY、resultX、resultY中****/
    /************************************************************************************/

    // 决策结束，向平台输出决策结果

    std::cout << startX << ' ' << startY << ' ' << resultX<< ' '<< resultY<< std::endl;
    return 0;
}