//
// Created by 林念祖 on 2024/4/11.
//

#ifndef CONNECTSIXBOT_EVALUATE_H
#define CONNECTSIXBOT_EVALUATE_H
#include <iostream>
#include <vector>
using namespace std;

vector<int>numberOfMyRoad(10);
vector<int>numberOfEnemyRoad(10);
vector<vector<int>>scoreOfMyRoad(2,vector<int>(10));
vector<vector<int>>scoreOfEnemyRoad(2,vector<int>(10));
#define ALERTSCORE ???;
#define GRIDSIZE 15
#define grid_blank 0
#define grid_black 1
#define grid_white -1
int baseScore = INT_MIN;

//基于路的局部扫描评估函数

int analysisHorizon(int currBotColor,Turn move) {
    //纵向扫描。黑棋加一分，白棋加7分
    if(isValued(move.x0,move.y0)){
        int x = move.x0;
        int y = move.y0;
        for (int i = x - 5 > 0? x- 5: 0; i < GRIDSIZE - 5 && i <= x; i++) {
            int number = data[i][y] + data[i + 1][y] + data[i + 2][y]
                         + data[i + 3][y] + data[i + 4][y] + data[i + 5][y];
            if (number == 0 || (number > 6 && number % 7 != 0)) {//判断是否没棋或者有黑有白
                continue;
            }
            if (number < 7) {
                if (currBotColor == grid_black) {
                    numberOfMyRoad[number]++;
                } else {
                    numberOfEnemyRoad[number]++;
                }

            } else {
                if (currBotColor == grid_black) {
                    numberOfEnemyRoad[number / 7]++;
                } else {
                    numberOfMyRoad[number / 7]++;
                }
            }
        }
    }
    if(isValued(move.x1,move.y1)){
        int x = move.x1;
        int y = move.y1;
        for (int i = x - 5 > 0? x- 5: 0; i < GRIDSIZE - 5 && i <= x; i++) {
            int xx = move.x0;
            if(i == xx|| i+1 == xx|| i + 2 == xx || i + 3 == xx || i + 4 == xx||i + 5 == xx)
                continue;//去重复
            int number = data[i][y] + data[i + 1][y] + data[i + 2][y]
                         + data[i + 3][y] + data[i + 4][y] + data[i + 5][y];
            if (number == 0 || (number > 6 && number % 7!= 0)) {//判断是否没棋或者有黑有白
                continue;
            }
            if (number < 7) {
                if (currBotColor == grid_black) {
                    numberOfMyRoad[number]++;
                } else {
                    numberOfEnemyRoad[number]++;
                }

            } else {
                if (currBotColor == grid_black) {
                    numberOfEnemyRoad[number / 7]++;
                } else {
                    numberOfMyRoad[number / 7]++;
                }
            }
        }
    }
}
int analysisVertical(int currBotColor,Turn move) {
    //横向扫描。黑棋加一分，白棋加7分
    if(isValued(move.x0,move.y0)){
        int x = move.x0;
        int y = move.y0;
        for (int i = y - 5 > 0? y- 5: 0; i < GRIDSIZE - 5 && i <= y; i++) {
            int number = data[x][i] + data[x][i + 1] + data[x][i + 2]
                         + data[x][i + 3] + data[x][i + 4] + data[x][i + 5];
            if (number == 0 || (number > 6 && number % 7 != 0)) {//判断是否没棋或者有黑有白
                continue;
            }
            if (number < 7) {
                if (currBotColor == grid_black) {
                    numberOfMyRoad[number]++;
                } else {
                    numberOfEnemyRoad[number]++;
                }

            } else {
                if (currBotColor == grid_black) {
                    numberOfEnemyRoad[number / 7]++;
                } else {
                    numberOfMyRoad[number / 7]++;
                }
            }
        }
    }
    if(isValued(move.x1,move.y1)){
        int x = move.x1;
        int y = move.y1;
        for (int i = y - 5 > 0? y- 5: 0; i < GRIDSIZE - 5 && i <= y; i++) {
            int xx = move.y0;
            if(i == xx|| i+1 == xx|| i + 2 == xx || i + 3 == xx || i + 4 == xx||i + 5 == xx)
                continue;//去重复
            int number = data[x][i] + data[x][i + 1] + data[x][i + 2]
                         + data[x][i + 3] + data[x][i + 4] + data[x][i + 5];
            if (number == 0 || (number > 6 && number % 7!= 0)) {//判断是否没棋或者有黑有白
                continue;
            }
            if (number < 7) {
                if (currBotColor == grid_black) {
                    numberOfMyRoad[number]++;
                } else {
                    numberOfEnemyRoad[number]++;
                }

            } else {
                if (currBotColor == grid_black) {
                    numberOfEnemyRoad[number / 7]++;
                } else {
                    numberOfMyRoad[number / 7]++;
                }
            }
        }
    }
}
int analysisLeft(int currBotColor,Turn move) {
    //向左下扫描。黑棋加一分，白棋加7分
    if(isValued(move.x0,move.y0)){
        int x = move.x0;
        int y = move.y0;
        for (int i = x + 5 < GRIDSIZE - 1? x+ 5: GRIDSIZE - 1,j = y - 5 > 0?y - 5: 0; i >= 5 && i >= x; i--,j++) {
            int number = data[i][j] + data[i - 1][j + 1] + data[i - 2][j + 2]
                         + data[i - 3][j + 3] + data[i - 4][j + 4] + data[i - 5][j + 5];
            if (number == 0 || (number > 6 && number % 7 != 0)) {//判断是否没棋或者有黑有白
                continue;
            }
            if (number < 7) {
                if (currBotColor == grid_black) {
                    numberOfMyRoad[number]++;
                } else {
                    numberOfEnemyRoad[number]++;
                }

            } else {
                if (currBotColor == grid_black) {
                    numberOfEnemyRoad[number / 7]++;
                } else {
                    numberOfMyRoad[number / 7]++;
                }
            }
        }
    }
    if(isValued(move.x1,move.y1)){
        int x = move.x1;
        int y = move.y1;
        for (int i = x + 5 < GRIDSIZE - 1? x+ 5: GRIDSIZE - 1,j = y - 5 > 0?y - 5: 0; i >= 5 && i >= x; i--,j++) {
            int xx = move.x0;
            int yy = move.y0;
            if((i==xx&&j==yy)||(i-1==xx&&j+1==yy)||(i-2==xx&&j+2==yy)||(i-3==xx&&j+3==yy)||(i-4==xx&&j+4==yy)||(i-5==xx&&j+5==yy))
                continue;//去重复
            int number = data[i][j] + data[i - 1][j+1] + data[i - 2][j + 2]
                         + data[i - 3][j + 3] + data[i - 4][j + 4] + data[i - 5][j + 5];
            if (number == 0 || (number > 6 && number % 7!= 0)) {//判断是否没棋或者有黑有白
                continue;
            }
            if (number < 7) {
                if (currBotColor == grid_black) {
                    numberOfMyRoad[number]++;
                } else {
                    numberOfEnemyRoad[number]++;
                }

            } else {
                if (currBotColor == grid_black) {
                    numberOfEnemyRoad[number / 7]++;
                } else {
                    numberOfMyRoad[number / 7]++;
                }
            }
        }
    }
}
int analysisRight(int currBotColor,Turn move) {
    //向右下扫描。黑棋加一分，白棋加7分
    if(isValued(move.x0,move.y0)){
        int x = move.x0;
        int y = move.y0;
        for (int i = x - 5 > 0? x- 5: 0,j = y - 5 > 0? y - 5: 0; i < GRIDSIZE - 5 && i <= x; i++,j++) {
            int number = data[i][j] + data[i + 1][j+1] + data[i + 2][j + 2]
                         + data[i + 3][ j + 3] + data[i + 4][j + 4] + data[i + 5][j + 5];
            if (number == 0 || (number > 6 && number % 7 != 0)) {//判断是否没棋或者有黑有白
                continue;
            }
            if (number < 7) {
                if (currBotColor == grid_black) {
                    numberOfMyRoad[number]++;
                } else {
                    numberOfEnemyRoad[number]++;
                }

            } else {
                if (currBotColor == grid_black) {
                    numberOfEnemyRoad[number / 7]++;
                } else {
                    numberOfMyRoad[number / 7]++;
                }
            }
        }
    }
    if(isValued(move.x1,move.y1)){
        int x = move.x1;
        int y = move.y1;
        for (int i = x - 5 > 0? x- 5: 0,j = y - 5 > 0?y - 5 : 0; i < GRIDSIZE - 5 && i <= x; i++,j++) {
            int xx = move.x0;
            int yy = move.y0;
            if((i==xx&&j==yy)||(i+1==xx&&j+1==yy)||(i+2==xx&&j+2==yy)||(i+3==xx&&j+3==yy)||(i+4==xx&&j+4==yy)||(i+5== xx&&j+5==yy))
                continue;//去重复
            int number = data[i][j] + data[i + 1][j + 1] + data[i + 2][j + 2]
                         + data[i + 3][j + 3] + data[i + 4][j + 4] + data[i + 5][j + 5];
            if (number == 0 || (number > 6 && number % 7!= 0)) {//判断是否没棋或者有黑有白
                continue;
            }
            if (number < 7) {
                if (currBotColor == grid_black) {
                    numberOfMyRoad[number]++;
                } else {
                    numberOfEnemyRoad[number]++;
                }

            } else {
                if (currBotColor == grid_black) {
                    numberOfEnemyRoad[number / 7]++;
                } else {
                    numberOfMyRoad[number / 7]++;
                }
            }
        }
    }
}

//基于路的全局扫描评估函数

int analysisVertical_(int currBotColor) {
    //横向扫描。黑棋加一分，白棋加7分
    for (int i = 0; i < GRIDSIZE; i++) {
        for (int j = 0; j < GRIDSIZE - 5; j++) {
            int number = data[i][j] + data[i][j + 1] + data[i][j + 2]
                         + data[i][j + 3] + data[i][j + 4] + data[i][j + 5];
            if (number == 0 || (number > 6 && number % 7!= 0)) {//判断是否没棋或者有黑有白
                continue;
            }
            if (number < 7) {
                if (currBotColor == grid_black) {
                    numberOfMyRoad[number]++;
                } else {
                    numberOfEnemyRoad[number]++;
                }

            } else {
                if (currBotColor == grid_black) {
                    numberOfEnemyRoad[number / 7]++;
                } else {
                    numberOfMyRoad[number / 7]++;
                }
            }
        }
    }
}
int analysisHorizon_(int currBotColor) {
    //纵向扫描。黑棋加一分，白棋加7分
    for (int i = 0; i < GRIDSIZE; i++) {
        for (int j = 0; j < GRIDSIZE - 5; j++) {
            int number = data[j][i] + data[j+1][i] + data[j+2][i]
                         + data[j+3][i] + data[j+4][i] + data[j+5][i];
            if (number == 0 || (number > 6 && number % 7!= 0)) {//判断是否没棋或者有黑有白
                continue;
            }
            if (number < 7) {
                if (currBotColor == grid_black) {
                    numberOfMyRoad[number]++;
                } else {
                    numberOfEnemyRoad[number]++;
                }

            } else {
                if (currBotColor == grid_black) {
                    numberOfEnemyRoad[number / 7]++;
                } else {
                    numberOfMyRoad[number / 7]++;
                }
            }
        }
    }
}

int analysisRight_(int currBotColor) {
    //向右下扫描。黑棋加一分，白棋加7分
    for (int i = 0; i < GRIDSIZE - 5; i++) {
        for (int j = 0; j < GRIDSIZE - 5; j++) {
            int number = data[i][j] + data[i+1][j + 1] + data[i+2][j + 2]
                         + data[i+3][j + 3] + data[i+4][j + 4] + data[i+5][j + 5];
            if (number == 0 || (number > 6 && number % 7!= 0)) {//判断是否没棋或者有黑有白
                continue;
            }
            if (number < 7) {
                if (currBotColor == grid_black) {
                    numberOfMyRoad[number]++;
                } else {
                    numberOfEnemyRoad[number]++;
                }

            } else {
                if (currBotColor == grid_black) {
                    numberOfEnemyRoad[number / 7]++;
                } else {
                    numberOfMyRoad[number / 7]++;
                }
            }
        }
    }
}
int analysisLeft_(int currBotColor) {
    //向坐下扫描。黑棋加一分，白棋加7分
    for (int i = GRIDSIZE - 1; i >= 5; i--) {
        for (int j = GRIDSIZE; j >= 5; j--) {
            int number = data[i][j] + data[i - 1][j - 1] + data[i - 1][j + 2]
                         + data[i - 3][j - 3] + data[i - 4][j - 4] + data[i - 5][j - 5];
            if (number == 0 || (number > 6 && number % 7!= 0)) {//判断是否没棋或者有黑有白
                continue;
            }
            if (number < 7) {
                if (currBotColor == grid_black) {
                    numberOfMyRoad[number]++;
                } else {
                    numberOfEnemyRoad[number]++;
                }

            } else {
                if (currBotColor == grid_black) {
                    numberOfEnemyRoad[number / 7]++;
                } else {
                    numberOfMyRoad[number / 7]++;
                }
            }
        }
    }
}

//计算此时局面的总分数(局部扫描)

int calScore(int currBotColor,Turn move){
    int score = 0;
    int condition = 0;
    if (baseScore > ALERTSCORE) {
        condition = 1;//condition用于表示进攻型还是防守型
    }
    for(int i = 1; i < 7; i++) {
        numberOfEnemyRoad[i] = 0;
        numberOfMyRoad[i] = 0;
    }
    //局部扫描，得到各种路的数量
    analysisHorizon(currBotColor, move);
    analysisVertical(currBotColor, move);
    analysisLeft(currBotColor, move);
    analysisRight(currBotColor, move);
    for(int i = 1; i < 7; i++) {
        score += (numberOfMyRoad[i] * scoreOfMyRoad[condition][i] -
                  numberOfEnemyRoad[i] *
                  scoreOfEnemyRoad[condition][i]);
    }
    return score;
}
//计算此时局面的总分数，得到baseScore(全局扫描)
int calScore_(int currBotColor){
    int score = 0;
    int condition = 0;
    if (baseScore > ALERTSCORE) {
        condition = 1;//condition用于表示进攻型还是防守型,1进攻，0防守
    }
    for(int i = 1; i < 7; i++) {
        numberOfEnemyRoad[i] = 0;
        numberOfMyRoad[i] = 0;
    }
    //全局扫描，得到各种路的数量
    analysisHorizon_(currBotColor);
    analysisVertical_(currBotColor);
    analysisLeft_(currBotColor);
    analysisRight_(currBotColor);
    for(int i = 1; i < 7; i++) {
        score += (numberOfMyRoad[i] * scoreOfMyRoad[condition][i] -
                  numberOfEnemyRoad[i] *
                  scoreOfEnemyRoad[condition][i]);
    }
    return score;
}

//给每种路赋值
void InitScore(){
    //防守型
    scoreOfMyRoad[0][1]=1;
    scoreOfMyRoad[0][2]=5;
    scoreOfMyRoad[0][3]=10;
    scoreOfMyRoad[0][4]=25;
    scoreOfMyRoad[0][5]=25;
    scoreOfMyRoad[0][6]=10000;
    scoreOfEnemyRoad[0][1]=1;
    scoreOfEnemyRoad[0][2]=5;
    scoreOfEnemyRoad[0][3]=15;
    scoreOfEnemyRoad[0][4]=35;
    scoreOfEnemyRoad[0][5]=25;
    scoreOfEnemyRoad[0][6]=10000;
    //进攻型
    scoreOfMyRoad[1][1]=1;
    scoreOfMyRoad[1][2]=15;
    scoreOfMyRoad[1][3]=30;
    scoreOfMyRoad[1][4]=50;
    scoreOfMyRoad[1][5]=50;
    scoreOfMyRoad[1][6]=10000;
    scoreOfEnemyRoad[1][1]=1;
    scoreOfEnemyRoad[1][2]=10;
    scoreOfEnemyRoad[1][3]=15;
    scoreOfEnemyRoad[1][4]=35;
    scoreOfEnemyRoad[1][5]=25;
    scoreOfEnemyRoad[1][6]=10000;
};

//所下棋子的价值：下棋后局面分数 - 下棋前局面分数
int evalute(int currBotColor, Turn move) {
    baseScore = calScore_(currBotColor);
    int fScore = 0;
    int bScore = 0;

    InitScore();/*建议在主函数中初始化*/

    fScore = calScore(currBotColor, move);
    makeMove(move, currBotColor);
    bScore = calScore(currBotColor, move);
    unMakeMove(move);
    return bScore - fScore;
}

#endif //CONNECTSIXBOT_EVALUATE_H
