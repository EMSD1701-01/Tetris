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
	//初始化游戏界面
	print_start_interface();

	//获取任意字符，游戏开始
	int ch = getch();

	//printf("getch: %c,%d\n",ch,ch);
	//在指定位置出现一个随机生成一个 图形
	srand(time(NULL));
	//num = random()%(max-min+1)+min;
	num = random()%(6-0+1)+0;
	mode = random()%(3-0+1)+0;
	color = random()%(46-41+1)+41;

	x = i_x;
	y = i_y;
	print_mode_shape(num,mode,x,y,color);

	//在下一图形框中，输出下一个图形
	print_next();
}

int main()
{
	//游戏初始化
	init_game();

	//注册信号
	//signal(SIGALRM,catch_signal);
	//开启定时器
	//alarm_us(tm);

	key_control();

	return 0;
}
