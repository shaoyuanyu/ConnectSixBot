
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>



#define GRIDSIZE 15
#define judge_black 0
#define judge_white 1
#define grid_blank 0
#define grid_black 1
#define grid_white -1

using namespace std;



int currBotColor; // 本方所执子颜色（1为黑，-1为白，棋盘状态亦同）
int gridInfo[GRIDSIZE][GRIDSIZE] = { 0 }; // 先x后y，记录棋盘状态
int dx[] = { -1,-1,-1,0,0,1,1,1 };
int dy[] = { -1,0,1,-1,1,-1,0,1 };
int startX,startY,resultX,resultY;
struct Local{
    int x1;
    int y1;
    Local (int x0,int y0){
        x1 = x0;
        y1= y0;
    }
};
void swap(int&a,int&b){
    int temp = a;
    a = b;
    b = temp;
}

// 判断是否在棋盘内
inline bool inMap(int x, int y)
{
    if (x < 0 || x >= GRIDSIZE || y < 0 || y >= GRIDSIZE)
        return false;
    return true;
}


// 在坐标处落子，检查是否合法或模拟落子
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
//评估函数
int getValue(int x0,int y0){
    int count=0;
      for(int i = 0; i<6; i++){
          int m1 = x0+i;
          int m2 = x0-i;
          if(m1 < GRIDSIZE){
              if(gridInfo[x0][y0] == gridInfo[m1][y0])
                  count++;
          }
          if(m2>=0 ){
              if(gridInfo[x0][y0] == gridInfo[m2][y0])
                  count++;
          }
      }
    for(int i = 0; i<6; i++){
        int m1 = y0+i;
        int m2 = y0-i;
        if(m1 < GRIDSIZE){
            if(gridInfo[x0][y0] == gridInfo[x0][m1])
                count++;
        }
        if(m2>=0 ){
            if(gridInfo[x0][y0] == gridInfo[x0][m2])
                count++;
        }
    }
    for(int i = 0; i<6; i++){
        int m1 = x0+i;
        int m2 = x0-i;
        int n1 = y0+i;
        int n2 = y0-i;
        if(m1 < GRIDSIZE && n1 < GRIDSIZE){
            if(gridInfo[x0][y0] == gridInfo[m1][n1])
                count++;
        }
        if(m2>=0 && n2 >= 0){
            if(gridInfo[x0][y0] == gridInfo[m2][n2])
                count++;
        }
    }

    return count;
}

//极大极小值
int miniMax(int alpha,int beta,bool isMax,int x0,int y0,int& x,int& y){
    int val1 = getValue(x0,y0);
    if(val1 >= 6)return val1;
    if(isMax){
        int bestScore = -1e5;
        for(int i= 0; i< GRIDSIZE; i++){
            for(int j = 0 ;j< GRIDSIZE; j++){
                if(gridInfo[i][j] == grid_blank ){
                        gridInfo[i][j] = currBotColor;
                        bestScore = max(bestScore, miniMax(alpha,beta,!isMax,i,j,x,y));
                        gridInfo[i][j] = grid_blank;
                        alpha = max(alpha,bestScore);
                        if(beta <= alpha)break;
                }
            }
        }
        return bestScore;
    }
    else {
        int bestScore = 1e5;
        for (int i = 0; i < GRIDSIZE; i++) {
            for (int j = 0; j < GRIDSIZE; j++) {
                if (gridInfo[i][j] == grid_blank) {
                    gridInfo[i][j] = currBotColor;
                    bestScore = min(bestScore, miniMax(alpha, beta, !isMax, i, j, x, y));
                    beta = min(beta, bestScore);
                    gridInfo[i][j] = grid_blank;
                    if (beta <= alpha)break;
                }
            }
        }
        return bestScore;
    }
}
void findBestMove(int&x0,int&y0){
    int val = -1e5;
    for(int i = 0; i < GRIDSIZE; i++){
        for(int j = 0; j < GRIDSIZE; j++){
            if(gridInfo[i][j] == grid_blank){
                gridInfo[i][j] = currBotColor;
                int alpha = -1e5;
                int beta = 1e5;
                int tempVal = miniMax(alpha,beta,false,0,0,x0,y0);
                gridInfo[i][j] = grid_blank;
                if(tempVal > val){
                    x0 = i;
                    y0 = j;
                    val = tempVal;
                }
            }
        }
    }
}

//做出决定
void makedecesion(){
    int flag = 0 ;
    for(int i = 0 ; i < GRIDSIZE; i++){
        for(int j = 0; j < GRIDSIZE; j++){
            if(gridInfo[i][j] != grid_blank)
                flag++;
            if(flag>0)break;
        }
    }
    if(flag>0){
        findBestMove(startX,startY);
        gridInfo[startX][startY] = currBotColor;
        findBestMove(resultX,resultY);
        gridInfo[resultX][resultY] = currBotColor;
    }
    else{
        findBestMove(startX,startY);
        gridInfo[startX][startY] = currBotColor;
        resultY = -1;
        resultX = -1;
    }

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
    makedecesion();
    cout << startX << ' ' << startY << ' ' << resultX<< ' '<< resultY<< endl;
    return 0;
}