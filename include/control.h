#ifndef _CONTROL_H_
#define _CONTROL_H_

//��ʱ�����
extern int tm;

//΢�ʱ��
void alarm_us(int t);

//�ź�ע�ắ��
void catch_signal(int signo);

int move_shape_down(int n,int m,int c);

void key_control();

void close_alarm();

#endif
