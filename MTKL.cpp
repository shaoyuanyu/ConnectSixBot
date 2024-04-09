//
// Created by 林念祖 on 2024/4/9.
//
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>


using namespace std;

#define GRIDSIZE 15
#define judge_black 0
#define judge_white 1
#define grid_blank 0
#define grid_black 1
#define grid_white -1



const int WINNING_LENGTH = 4;
const char PLAYER_PIECE = 'X';
const char COMPUTER_PIECE = 'O';

struct Move {
    int row, col;
    Move() {}
    Move(int r, int c) : row(r), col(c) {}
};



int currBotColor; // 本方所执子颜色（1为黑，-1为白，棋盘状态亦同）
int gridInfo[GRIDSIZE][GRIDSIZE] = { 0 }; // 先x后y，记录棋盘状态

//int dx[] = { -1,-1,-1,0,0,1,1,1 };
//int dy[] = { -1,0,1,-1,1,-1,0,1 };

// 判断是否在棋盘内
inline bool inMap(int x, int y)
{
    if (x < 0 || x >= GRIDSIZE || y < 0 || y >= GRIDSIZE)
        return false;
    return true;
}
bool ProcStep(int x0, int y0, int x1, int y1, int grid_color, bool check_only)
{
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


bool isWinningMove(int row, int col, int piece) {
    // 定义四个方向：水平、垂直、对角线（从左上到右下）、反对角线（从右上到左下）
    int dx[] = {1, 0, 1, 1};
    int dy[] = {0, 1, 1, -1};

    for (int dir = 0; dir < 4; ++dir) {
        int count = 1; // 当前位置的棋子已经包含在内，初始化为1

        // 在每个方向上，分别向前后两个方向遍历
        for (int k = 1; k <= WINNING_LENGTH; ++k) {
            int newRow = row + k * dx[dir];
            int newCol = col + k * dy[dir];

            // 检查是否越界
            if (newRow < 0 || newRow >= GRIDSIZE || newCol < 0 || newCol >= GRIDSIZE) break;

            // 检查是否与当前棋子相同
            if (gridInfo[newRow][newCol] == piece) {
                count++;
            } else {
                break; // 如果不相同，停止向该方向继续检查
            }
        }

        // 向反方向继续检查
        for (int k = 1; k <= WINNING_LENGTH; ++k) {
            int newRow = row - k * dx[dir];
            int newCol = col - k * dy[dir];

            // 检查是否越界
            if (newRow < 0 || newRow >= GRIDSIZE || newCol < 0 || newCol >= GRIDSIZE) break;

            // 检查是否与当前棋子相同
            if (gridInfo[newRow][newCol] == piece) {
                count++;
            } else {
                break; // 如果不相同，停止向该方向继续检查
            }
        }

        // 如果在任何一个方向上找到了连续的相同棋子数量达到或超过获胜长度，返回true
        if (count >= WINNING_LENGTH) return true;
    }

    return false;
}

void makeMove(int row, int col, int piece,int board[][GRIDSIZE]) {
    board[row][col] = piece;
}

bool isFull() {
    for (int i = 0; i < GRIDSIZE; ++i) {
        for (int j = 0; j < GRIDSIZE; ++j) {
            if (gridInfo[i][j] == grid_blank) return false;
        }
    }
    return true;
}


int UCT(){}

int simulate(int simulatedBoard[][GRIDSIZE], int currentPiece, int simulations) {
    int wins = 0;

    for (int i = 0; i < simulations; ++i) {
        auto tempBoard = simulatedBoard;

        // 随机进行模拟游戏
        while (!isFull()) {
            int row = rand() % GRIDSIZE;
            int col = rand() % GRIDSIZE;
            if (isValidMove(row, col)) {
                makeMove(row, col, currentPiece,tempBoard);
                if (isWinningMove(row, col, currentPiece)) {
                    if (currentPiece == computerPiece) wins++;
                    break;
                }
                currentPiece = (currentPiece == grid_black) ? grid_white : grid_black;
            }
        }
    }

    return wins;
}

Move getBestMove(int piece) {
    srand(time(NULL));

    vector<Move> availableMoves;
    for (int i = 0; i < GRIDSIZE; ++i) {
        for (int j = 0; j < GRIDSIZE; ++j) {
            if (uct(i,j)) {
                availableMoves.push_back(Move(i, j));
            }
        }
    }

    int bestScore = -1;
    Move bestMove;

    for (const Move &move : availableMoves) {
        auto tempBoard = gridInfo;
        makeMove(move.row, move.col, piece,tempBoard);
        int score = simulate(tempBoard, piece, 1000); // 蒙特卡洛模拟1000次
        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }
    return bestMove;
}







int main()
{
    int x0, y0, x1, y1;

    // 分析自己收到的输入和自己过往的输出，并恢复棋盘状态
    int turnID;
    cin >> turnID;
    currBotColor = grid_white; // 先假设自己是白方
    for (int i = 0; i < turnID; i++)
    {
        // 根据这些输入输出逐渐恢复状态到当前回合
        cin >> x0 >> y0 >> x1 >> y1;
        if (x0 == -1)
            currBotColor = grid_black; // 第一回合收到坐标是-1, -1，说明我是黑方
        if (x0 >= 0)
            ProcStep(x0, y0, x1, y1, -currBotColor, false); // 模拟对方落子
        if (i < turnID - 1) {
            cin >> x0 >> y0 >> x1 >> y1;
            if (x0 >= 0)
                ProcStep(x0, y0, x1, y1, currBotColor, false); // 模拟己方落子
        }
    }

    int startX, startY, resultX, resultY;








    cout << startX << ' ' << startY << ' ' << resultX<< ' '<< resultY<< endl;
    return 0;
}






































