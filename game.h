/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   game.h
 * Author: karrasmi
 *
 * Created on March 20, 2017, 12:33 PM
 */

#ifndef GAME_H
#define GAME_H

#ifdef __cplusplus
extern "C" {
#endif
#include <math.h>
#include <stdbool.h>
void evalBoard(char board[][26],int n);
bool positionInBounds(char board[][26], int n, char row, char col);
bool checkLegalInDirection(char board[][26], int n, int  row, int col,
    char colour, int deltaRow, int deltaCol);
bool checkplace(char board[][26], int i, int j, int n, char colour, bool on);
bool checkmoves(char colour, int n, char board[][26]);
void printBoard(char board[][26], int n);
void initBoard(char board[][26],int  n);
bool terminalBoard(char board[][26],int n);


#ifdef __cplusplus
}
#endif

#endif /* GAME_H */

