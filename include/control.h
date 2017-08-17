#ifndef _CONTROL_H_
#define _CONTROL_H_

//定时器间隔
extern int tm;
extern struct termios tm_old;
extern int num,mode,color;
extern int n_num,n_mode,n_color;
extern int i_x,i_y;
extern int x,y;
extern int s_x,s_y,l_x,l_y;
extern int shape[7][4][18];

//微妙定时器
void alarm_us(int t);

//信号注册函数
void catch_signal(int signo);

void key_control();

void close_alarm();

int getch();

#endif
