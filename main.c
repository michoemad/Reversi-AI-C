/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: karrasmi
 *
 * Created on March 20, 2017, 12:32 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "algo.h"
#include "lab7part2lib.h"
#include <sys/time.h>
#include <sys/resource.h>

int main() {
    int player, n;
    move = 1;
    char other, myColor;
    char board[26][26];
    int static_Grid[26][26];
    printf("Enter the board dimension: ");
    scanf("%d", &n);
    // init_Static(static_Grid, n);
    //print_Grid(n,static_Grid);
    //return 0;
    printf("Computer plays (B/W) : ");
    scanf(" %c", &other);
    //Decide who's playing first
    if (other == 'B') {
        player = -1;
        myColor = 'W';
    } else {
        player = 1;
        myColor = 'B';
    }
    initBoard(board, n);
    int i = 0;
    char inRow, inCol;
    bool compMove = 1, playMove = 1;
    printBoard(board, n);
    while (!terminalBoard(board, n)) {
        if (player == -1) {
            printf("Move # %d\n", move);
            struct rusage usage; // a structure to hold "resource usage" (including time)
            struct timeval start, end; // will hold the start and end times
            getrusage(RUSAGE_SELF, &usage);
            start = usage.ru_utime;
            double timeStart = start.tv_sec + start.tv_usec / 1000000.0; // in seconds
            //Computer's turn
            char tRow, tCol;
            if (IDD(board, &tRow, &tCol, n, other, static_Grid)) {
                getrusage(RUSAGE_SELF, &usage);
                end = usage.ru_utime;
                double timeEnd = end.tv_sec + end.tv_usec / 1000000.0; // in seconds
                double totalTime = timeEnd - timeStart;
                printf("Time is %lf\n", totalTime);
                if (totalTime >= 1) {
                    printf("TIME LIMIT\n");
                   return 0;
                }
                printf("Computer places %c at %c%c.\n", other, tRow, tCol);

                printBoard(board, n);
            } else {
                printf("%c player has no valid move.\n", other);
                compMove = 0;
            }
            move += 1;
        }else { // My turn
            // No moves found
            if (!(checkmoves(myColor, n, board))) {
                printf("%c player has no valid move.\n", myColor);
                player = -player;
                continue;
            }
            char row, col;
            printf("Enter move for colour %c (RowCol): ",myColor);
            scanf(" %c%c",&row,&col);
             if (!(checkplace(board,row-'a',col-'a',n,myColor,true))){
                printf("Invalid move.\n");
                printf("%c player wins.",other);
                //Terminate the game if my player enters a wrong move
                return 0;
            }
            else{
                printBoard(board,n);
                playMove=1;
            }
        //switch players
        player = (-1) * player;
        } }
    evalBoard(board, n);
    return 0;
}

