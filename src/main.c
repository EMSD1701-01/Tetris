#include <stdio.h>
#include "print.h"
#include <time.h>
#include <signal.h>
#include <stdlib.h>
#include "control.h"
extern int x,y;

#ifdef WIN32

#define random() rand()

#else

#endif 

void init_game()
{
	//��ʼ����Ϸ����
	print_start_interface();

	//��ȡ�����ַ�����Ϸ��ʼ
	int ch = getch();

	//printf("getch: %c,%d\n",ch,ch);
	//��ָ��λ�ó���һ���������һ�� ͼ��
	srand(time(NULL));
	//num = random()%(max-min+1)+min;
	num = random()%(6-0+1)+0;
	mode = random()%(3-0+1)+0;
	color = random()%(46-41+1)+41;

	x = i_x;
	y = i_y;
	print_mode_shape(num,mode,x,y,color);

	//����һͼ�ο��У������һ��ͼ��
	print_next();
}

int main()
{
	//��Ϸ��ʼ��
	init_game();

	//ע���ź�
	//signal(SIGALRM,catch_signal);
	//������ʱ��
	//alarm_us(tm);

	key_control();

	return 0;
}
