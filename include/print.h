#ifndef _PRINT_H_
#define _PRINT_H_



#include <stdio.h>



/**
 * 函数声明
 */
void clear();
int getch();
void print_start_interface();
void print_mode_shape(int n,int m,int x,int y,int c);
void eraser_shape(int,int,int,int);
void print_matrix();
void print_score_level();
void game_over();



/**
 * 从外部引用的变量
 */
extern int matrix[24][28];
extern int score, level;



#endif



