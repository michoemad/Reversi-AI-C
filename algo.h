/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   algo.h
 * Author: karrasmi
 *
 * Created on March 20, 2017, 12:32 PM
 */

#ifndef ALGO_H
#define ALGO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <sys/time.h>
#include <sys/resource.h>
#include  <signal.h>
int move;
bool organizer(char board[][26], char* tRow, char* tCol, int n, char other, int static_Grid[][26],int depth,double timeStart,bool* flag);
bool checkTime(double timeStart);
double getScore_Static(char board[][26],int n, int static_Grid[][26],char other );
void init_Static(int static_Grid[][26], int n);
void print_Grid(int n, int static_Grid[][26]);
void createTemp(char board[][26], char tempBoard[][26],int n);
double count(char board[][26],int n,char other);
double minMax(char board[][26], int depth, double alpha, double beta, int n, char other, int static_Grid[][26], bool player,double timeStart);
bool IDD(char board[][26], char* tRow, char* tCol, int n, char other,int static_Grid[][26]);
double dynamicEval(char board[][26], int n, char other);
int checkStable(char board[][26], int n,int i, int j);
int checker(char board[][26], int n, int row, int col, int deltaRow, int deltaCol);
char getOther(char player);
#ifdef __cplusplus
}
#endif

#endif /* ALGO_H */

