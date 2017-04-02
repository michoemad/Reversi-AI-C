/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "algo.h"
#define TRY do{  if( !setjmp(ex_buf__) ){
#define CATCH } else {
#define ETRY } }while(0)
#define THROW longjmp(ex_buf__, 1)
jmp_buf ex_buf__;

const int DEPTH = 6;
const double INF = 99999;

char getOther(char player) {
    if (player == 'W') {
        return 'B';
    } else {
        return 'W';
    }
}

//1 means stable, -1 means not stable, 0 means potentially

int checker(char board[][26], int n, int row, int col, int deltaRow, int deltaCol) {
    char k = board[row][col];
    int state1 = 0, state2 = 0; //1 = diff color, 2 = unallocated, 0 = same color (terminal)
    int i = 1;
    while (positionInBounds(board, n, row + deltaRow * i, col + deltaCol * i)) {
        if (board[row + deltaRow * i][col + deltaCol * i] == getOther(k)) {
            state1 = 1;
            break;
        } else if (board[row + deltaRow * i][col + deltaCol * i] == 'U') {
            state1 = 2;
            break;
        }
        i += 1;
    }
    if (state1 == 0) {
        return 1;
    } //implies stability
    //check other direction
    i = -1;
    while (positionInBounds(board, n, row + deltaRow * i, col + deltaCol * i)) {
        if (board[row + deltaRow * i][col + deltaCol * i] == getOther(k)) {
            state2 = 1;
            break;
        } else if (board[row + deltaRow * i][col + deltaCol * i] == 'U') {
            state2 = 2;
            break;
        }
        i -= 1;
    }
    if ((state1 == 1)&&(state2 == 1)) { //WBW => stable
        return 1;
    } else if (state1 * state2 == 2) { //handles if one of them is 2 and the other is 1 //WBU
        return -1;
    } else if (state1 * state2 == 0) {//handles if any of the states are 0 => stable
        return 1;
    } else {
        return 0;
    }

}

//1 indicates that it is unstable (can be changed in next move)

int checkStable(char board[][26], int n, int i, int j) {
    int V, H, D, D2;
    if (board[i][j] != 'U') {
        //check all axi
        //Vertical (+ve and -ve)
        V = checker(board, n, i, j, 1, 0);
        if (V != 1) {
            return 0;
        }
        H = checker(board, n, i, j, 0, 1);
        if (H != 1) {
            return 0;
        }
        D = checker(board, n, i, j, 1, 1);
        if (D != 1) {
            return 0;
        }

        D2 = checker(board, n, i, j, -1, 1);
        if (D2 != 1) {
            return 0;
        }
        if ((V == 1)&&(H == 1)&&(D == 1)&&(D2 == 1)) {
            return 1;
        }
    }
    return 0;
}

double dynamicEval(char board[][26], int n, char other) {
    double total = 0, C1 = 0, C2 = 0, corn1 = 0, corn2 = 0, stab1 = 0, stab2 = 0;
    double M1 = 0, M2 = 0;
    char minPlayer = getOther(other);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == other) {
                C1 += 1;
                int f = checkStable(board, n, i, j);
                if (f >= 0) {
                    stab1 += f;
                }
                //stab1+= checkStable(board, n, i, j);
            } else if (board[i][j] == minPlayer) {
                C2 += 1;
                int f = checkStable(board, n, i, j);
                if (f >= 0) {
                    stab2 += f;
                }
            }
            if (((i == 0)&&(j == 0)) || ((i == 0)&&(j == n - 1))&&((i == n - 1)&&(j == n - 1))&&((i == n - 1)&&(j == 0))) {
                if (board[i][j] == other) {
                    corn1 += 1;
                }
                if (board[i][j] == minPlayer) {
                    corn2 += 1;
                }
            }
            if (board[i][j] == 'U') {
                if (checkplace(board, i, j, n, other, false)) {
                    M1 += 1;
                }
                if (checkplace(board, i, j, n, minPlayer, false)) {
                    M2 += 1;
                }
            }
        }
    }
    double G = 0;
    if (M1 + M2 != 0) {
        G += 99 * ((M1 - M2) / (M1 + M2));
    }
    if (stab1 + stab2 != 0) {
        G += 36 * (stab1 - stab2) / (stab1 + stab2);
    }
    if (corn1 + corn2 != 0) {
        G += 50 * (corn1 - corn2) / (corn1 + corn2);
    }
    G += 20 * ((C1 - C2) / (C1 + C2));
    return G;

}

double count(char board[][26], int n, char other) {
    double C = 0;
    char myColor;
    if (other == 'W') {
        myColor = 'B';
    } else {
        myColor = 'W';
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == other) {
                C += 1;
            } else if (board[i][j] == myColor) {
                C -= 1;
            }
        }
    }
    //  printf("I AM");
    if (C > 0) {
        return INF;
    } else if (C < 0) {
        return -INF;
    }
    return 0;
}

void createTemp(char board[][26], char tempBoard[][26], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            tempBoard[i][j] = board[i][j];
        }
    }
}

void print_Grid(int n, int static_Grid[][26]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%2d ", static_Grid[i][j]);
        }
        printf("\n");
    }
}

void init_Static(int static_Grid[][26], int n) {
    //first two rows
    static_Grid[0][0] = 4;
    static_Grid[0][1] = -3;
    static_Grid[0][n - 1] = 4;
    static_Grid[0][n - 2] = -3;
    static_Grid[1][0] = -3;
    static_Grid[1][1] = -4;
    static_Grid[1][n - 1] = -3;
    static_Grid[1][n - 2] = -4;
    static_Grid[n - 1][0] = 4;
    static_Grid[n - 2][0] = -3;
    static_Grid[n - 1][n - 1] = 4;
    static_Grid[n - 1][n - 2] = -3;
    static_Grid[n - 1][1] = -3;
    static_Grid[n - 2][1] = -4;
    static_Grid[n - 2][n - 1] = -3;
    static_Grid[n - 2][n - 2] = -4;


    for (int i = 2; i < n - 2; i++) {
        static_Grid[i][0] = 2;
        static_Grid[i][n - 1] = 3;
        static_Grid[i][1] = -1;
        static_Grid[i][n - 2] = -1;

        static_Grid[0][i] = 3;
        static_Grid[1][i] = -1;
        static_Grid[n - 1][i] = 3;
        static_Grid[n - 2][i] = -1;
        for (int j = 2; j < n - 2; j++) {
            static_Grid[j][i] = 0;
        }
    }
    static_Grid[n / 2][n / 2] = 1;
    static_Grid[n / 2][(n / 2) - 1] = 1;
    static_Grid[(n / 2) - 1][(n / 2) - 1] = 1;
    static_Grid[(n / 2) - 1][(n / 2)] = 1;
    if (n >= 5) {
        for (int i = 2; i < n - 2; i++) {
            static_Grid[i][i] = 1;
            static_Grid[n - i - 1][i] = 1;
        }
        static_Grid[2][2] = 1;
        static_Grid[2][n - 3] = 1;
        static_Grid[n - 3][n - 3] = 1;
        static_Grid[n - 3][2] = 1;
    }

}

double getScore_Static(char board[][26], int n, int static_Grid[][26], char other) {
    char myColor;
    if (other == 'W') {
        myColor = 'B';
    } else {
        myColor = 'W';
    }
    int total = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == other) {
                total += static_Grid[i][j];
            }
            if (board[i][j] == myColor) {
                total -= static_Grid[i][j];
            }
        }
    }
    return total; //+dynamicEval(board,n,other);

}

//if player is on then I am maximiziG agent

double minMax(char board[][26], int depth, double alpha, double beta, int n, char other, int static_Grid[][26], bool player,double timeStart) {
    double maxScore = -INF, minScore = INF; //We're maximizing my bruddah
    bool move = false;
    char myColor;
    char copyBoard[26][26];
    memcpy(copyBoard, board, sizeof (board[0]) * n);
    myColor = getOther(other);
    //Get all the children
    if (terminalBoard(board, n)) {
        return count(board, n, other);
    }
    if ((depth <= 0)) {
        // printf("%d\n",depth);

        //return getScore_Static(board, n, static_Grid, other);
        //   printf("IN MIN MOVE IS %d\n",move);
        return dynamicEval(board, n, other);
    }
    // char tempBoard[26][26];
    //O(n^2)
    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            double score;
             if (checkTime(timeStart)){
                 return -INF;
             }
            if (player) {
                if (checkplace(copyBoard, row, col, n, other, true)) {
                    move = true;
                    score = minMax(copyBoard, depth, alpha, beta, n, other, static_Grid, !player,timeStart); //Holds the score at the end of the game
                    if (score > maxScore) {
                        maxScore = score;
                    }
                    if (maxScore > alpha) {
                        alpha = maxScore;
                    }
                    if (alpha >= beta) {
                        //stop this thing
                        return maxScore;
                    }
                    memcpy(copyBoard, board, sizeof (board[0]) * n);
                }
            } else {
                if (checkplace(copyBoard, row, col, n, myColor, true)) {
                    move = true;
                    score = minMax(copyBoard, depth - 1, alpha, beta, n, other, static_Grid, !player,timeStart);
                    if (score < minScore) {
                        minScore = score;
                    }
                    if (minScore < beta) {
                        beta = minScore;
                    }
                    if (alpha >= beta) {
                        return minScore;
                    }
                    memcpy(copyBoard, board, sizeof (board[0])*26);
                }
            }
        }
    }
    if (move) {
        if (player) {
            return maxScore;
        } else {
            return minScore;
        }
    } else {
        //No valid move available
        if (!player) {
            depth -= 1;
        }
        return minMax(board, depth, alpha, beta, n, other, static_Grid, !player,timeStart);
    }

}

bool organizer(char board[][26], char* tRow, char* tCol, int n, char other, int static_Grid[][26], int depth,double timeStart,bool* flag) {
    double maxScore = -INF, alpha = -INF, beta = INF, H = -INF;
    int bestI = -1, bestJ = -1, semiI = -1, semiJ = -1;
    char myColor = getOther(other);
    char copyBoard[26][26];
    memcpy(copyBoard, board, sizeof (board[0]) * n);

    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            if (checkplace(copyBoard, row, col, n, other, true)) { //means this is a valid place, we can get its children
                // the child
                // char (*boardTemp)[n] = board;
                double score = minMax(copyBoard, depth, alpha, beta, n, other, static_Grid, false,timeStart);
                //                double D = dynamicEval(copyBoard,n,other);
                //                if (D>H){H= D; semiI=row;semiJ=col;}
                if (score >= maxScore) {
                    maxScore = score;
                    bestI = row;
                    bestJ = col;
                    if (score > alpha) {
                        alpha = score;
                    }
                    if (maxScore == INF) {
                        goto A;
                    } //found a winning path
                }
                if (checkTime(timeStart)){
                    return true;
                }
                memcpy(copyBoard, board, sizeof (board[0]) * n);
            }
        }
    }
A:
    //printf("%.2lf %d %d\n", maxScore, bestI, bestJ);
    if (maxScore == -INF){*flag=true;}
    if ((bestI == -1) || (bestJ == -1)) {
        return false; // didn't find a move fam
    }

   // checkplace(board, bestI, bestJ, n, other, true);
   // printf("The greatest are %d %d\n", bestI, bestJ);
    *tRow = 'a' + bestI;
    *tCol = 'a' + bestJ;
    return true;


}


bool checkTime(double timeStart){
    struct rusage usage; // a structure to hold "resource usage" (including time)
    struct timeval start, end; // will hold the start and end times
    getrusage(RUSAGE_SELF, &usage);
    end = usage.ru_utime;
    double timeEnd = end.tv_sec + end.tv_usec / 1000000.0; // in seconds
    double totalTime = timeEnd - timeStart;
    return totalTime>=0.9;
}

bool IDD(char board[][26], char* tRow, char* tCol, int n, char other,int static_Grid[][26]) {
   // signal(SIGALRM, sigHandler);
    char pRow=-1,pCol=-1;
            struct rusage usage; // a structure to hold "resource usage" (including time)
            struct timeval start, end; // will hold the start and end times
            getrusage(RUSAGE_SELF, &usage);
            start = usage.ru_utime;
            double timeStart = start.tv_sec + start.tv_usec / 1000000.0; // in seconds
   // setitimer(ITIMER_PROF, &it_val, NULL);
    int d;

    for( d = 1;d<=DEPTH;d++){
        bool flag =false;
        char row,col;
        if (!organizer(board, &row, &col, n, other, static_Grid, d,timeStart,&flag)){ //handles the case where there is no possible move
            return false;
        }
        if (checkTime(timeStart)){
            break;
        }
        if (!((flag)&&(pRow!=-1)&&(pCol!=-1))){
            pRow=row;pCol=col;
        }
        printf("At depth %d\n",d);
    }
    
    *tRow=pRow;*tCol=pCol;
    checkplace(board, pRow-'a', pCol-'a', n, other, true);
    printf("Last depth %d\n",d);
    return true;

}
