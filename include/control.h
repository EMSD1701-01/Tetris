#ifndef _CONTROL_H_
#define _CONTROL_H_



#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#ifdef WIN32 //-----Windows------

#include <windows.h>
#define random() rand()

#else //--------------Linux-------

#include <signal.h>
#include <linux/input.h>
#include <termios.h>

#endif //WIN32----------------

/**
 * 全局函数声明
 */
void init_game();
void start_game();



/**
 * 从外部应用变量
 */
extern int shape[7][4][18];
extern int i_x, i_y;
extern int n_x, n_y;
// extern int p_x = 60,p_y = 15;



#endif
