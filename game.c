/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#ifndef game_H_INCLUDED
#define game_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>

#include "game.h"

//return 1 if White wins, -1 if black wins, 0 for tie
void evalBoard(char board[][26],int n){
    int scoreW =0, scoreB=0;
    for(int i =0;i<n;i++){
        for(int j =0;j<n;j++){
            if (board[i][j] == 'W'){
                scoreW+=1;
            }
            else if (board[i][j]=='B'){
                scoreB+=1;
            }
        }
    }
    if (scoreW>scoreB){
        printf("W player wins.");
    }
    else if (scoreB>scoreW){
       printf("B player wins.");
    }
    else{
        //Tie
        printf("Draw!");
    }
}

bool positionInBounds(char board[][26], int n, char row, char col) {
    return ((row < n)&&(col < n) &&(row >= 0) && (col >= 0));
}


bool checkLegalInDirection(char board[][26], int n, int  row, int col,
    char colour, int deltaRow, int deltaCol) {
    //where col is the color of the tile to be placed
    //int i =1;
    char other;
    bool on = false;
    //This variable checks if there exists a different color in the direction specified
    if (colour == 'W') {
        other = 'B';
    } else {
        other = 'W';
    }
    row += deltaRow;
    col += deltaCol;
    //Loop over the deltarow and deltacol
    while ((positionInBounds(board, n, row, col)) && (board[row][col] == other)) {
        //printf("%d %d\n",row,col);
        row += deltaRow;
        col += deltaCol;
        on = true;
    }
    //Returns true if the colour is found
    if ((positionInBounds(board, n, row, col)) &&(on)) {
        if (board[row][col] == colour) {

            //printf("%d %d %d %d\n",deltaRow,deltaCol,row,col);
            return true;
        }
    }
    return false;

}




//The bool on determines whether the player is actually playing a move

bool checkplace(char board[][26], int i, int j, int n, char colour, bool on) {
    bool valid = false;
    char other;
    //Basically who my opponent is given colour
    if (colour == 'W') {
        other = 'B';
    } else {
        other = 'W';
    }
    //For a move to be considered it has to be empty
    if ((board[i][j] == 'U') &&(positionInBounds(board, n, i, j))) {
        //Loop over all possible directions of movement
        for (int deltaRow = -1; deltaRow <= 1; deltaRow++) {
            for (int deltaCol = -1; deltaCol <= 1; deltaCol++) {
                if (!((deltaRow == deltaCol) &&(deltaRow == 0))) {
                    //Use checklegal to determine if that direction vector is applicable
                    if (checkLegalInDirection(board, n, i, j, colour, deltaRow, deltaCol)) {
                        valid = true;
                        if (on) { //This means replace the actual pieces, the player is playing the game
                            board[i][j]=colour;
                            int tI=i,tJ=j;
                            tI += deltaRow;
                            tJ += deltaCol;

                            //Change the colour of the opponent
                            while ((positionInBounds(board, n, tI, tJ)) && (board[tI][tJ] == other)) {
                                board[tI][tJ] = colour;
                                tI += deltaRow;
                                tJ += deltaCol;
                                // Switch tiles fam
                            }
                        } else {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return valid;
}


//This function checks all possible moves given the grid and the colour to play with
bool checkmoves(char colour, int n, char board[][26]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (checkplace(board, i, j, n, colour, false)) {
                // A move has been found
                return true;
            }
        }
    }
    return false;
}


//Prints the board given its dimensions in the specified format
void printBoard(char board[][26], int n) {
    printf("  ");
    //print row headers
    for (int i = 0; i < n; i++) {
        printf("%c", 'a' + i);
    }
    printf("\n");
    //loop ove rrows and columns to print each position
    for (int i = 0; i < n; i++) {
        printf("%c ", 'a' + i);
        for (int j = 0; j < n; j++) {
            printf("%c", board[i][j]);
        }
        printf("\n");
    }
}



void initBoard(char board[][26],int  n){
    //Initialize everything to unallocated
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            board[i][j] = 'U';
        }
    }

    //Those basic places for white and black
    board[n / 2][n / 2] = 'W';
    board[(n / 2) - 1][(n / 2) - 1] = 'W';
    board[(n / 2) - 1][n / 2] = 'B';
    board[n / 2][(n / 2) - 1] = 'B';
}

//This functions checks if a possible move is to be done with color "colour" in place (i,j) i for row, j for column
//Boolean function to check if a position did not get out of the board row is in [0,n) and col is [0,n)


//The function checks if the direction specified by deltarow and delta col is ok to work with, given the colour, row and column

//Let's see if the game is over
//Case 1: it's full
//Case 2: no one has moves
bool terminalBoard(char board[][26],int n){

    if ( (!(checkmoves('W',n,board))) && (!(checkmoves('B',n,board)))){
        // No moves for both manz
        //printf("NIHH");
        return true;
    }
    return false;
}




#endif
