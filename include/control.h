#ifndef _CONTROL_H_
#define _CONTROL_H_

//定时器间隔
extern int tm;

//微妙定时器
void alarm_us(int t);

//信号注册函数
void catch_signal(int signo);

int move_shape_down(int n,int m,int c);

void key_control();

void close_alarm();

#endif
