#include "control.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#ifdef WIN32 //-----Windows------

#include <windows.h>
#define random() rand()

#else //--------------Linux-------

#include <signal.h>
#include <linux/input.h>

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



/**
 * 全局变量
 */
int matrix[24][28] = {0};	//画面矩阵
int score = 0,level = 1;	//分数 等级


/**
 * 文件作用域函数声明
 */
static void alarm_us(int t);		//微妙定时器
static void catch_signal(int signo);	//信号注册函数
static void key_control();
static void close_alarm();
static int judge_shape(int n,int m,int x,int y);
static void store_shape();
static void new_shape();
static void destroy_line();
static int is_over();
static void move_shape_down();
static void fall_down();
static void move_shape_left();
static void move_shape_right();
static void change_shape();


/**
 * 文件作用域变量、宏
 */
#define GAMESTOP			1
#define GAMEON				2
#define GAMEPAUSE			3
#define GAMEOVER 			4
#define GAMEAGAIN			5

int num, mode, color;			//当前图形信息
int n_num, n_mode, n_color;		//下一图形信息
int x, y;				//图形当前位置信息
int tm = 800000;	//定时器间隔
int g_GameStatus = GAMESTOP;	//当前游戏状态



void init_game()
{
	g_GameStatus = GAMESTOP;
	srand(time(NULL));
	new_shape();
	new_shape();
	//初始化游戏界面
	print_start_interface();
	print_matrix();
	key_control();
}



void start_game()
{
	print_mode_shape(n_num, n_mode, n_x, n_y, n_color);

	g_GameStatus = GAMEON;
	print_mode_shape(num, mode, x, y, color);

	//注册信号
	//signal(SIGALRM,catch_signal);
	//开启定时器
	alarm_us(tm);
	//开始接受键盘消息
}


#ifdef WIN32 //----------------Windows----------------------

#define TIMER_ID 1
void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	catch_signal((int)dwTime);
	sleep(1);
}

DWORD CALLBACK ThreadProc(PVOID pvoid)
{
	//ǿ��ϵͳΪ�̼߳�����Ϣ����
	MSG msg;
	PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);

	SetTimer(0, TIMER_ID, 800, TimerProc);

	//��ȡ���ַ���Ϣ
	while(GetMessage(&msg, NULL, 0, 0))
	{
		if(msg.message == WM_TIMER)
		{
			TranslateMessage(&msg);    // ������Ϣ
			DispatchMessage(&msg);     // �ַ���Ϣ
		}
	}

	KillTimer(NULL, 10);
	printf("thread end here\n");
	return 0;
}

void alarm_us(int t)
{
	//KillTimer(NULL, TIMER_ID);
	//SetTimer(NULL, TIMER_ID, t / 1000, TimerProc);
}
void close_alarm()
{
	//KillTimer(NULL, TIMER_ID);
}

#else //-------------------------Linux-----------------------

//微妙定时器,定时器一旦启动，会每隔一段时间发送SIGALRM信号
static void alarm_us(int t)
{
	struct itimerval value;
	//定时器启动的初始值
	value.it_value.tv_sec = 0;
	value.it_value.tv_usec = t;

	//定时器启动后的间隔时间值
	value.it_interval.tv_sec = 0;
	value.it_interval.tv_usec = t;

	setitimer(ITIMER_REAL,&value,NULL);
}



//信号注册函数
static void catch_signal(int signo)
{
	//向下移动图形，一直到底部
	move_shape_down(num,mode,color);
	// signal(SIGALRM,catch_signal);
}


//关闭定时器
static void close_alarm()
{
	struct itimerval value;
	//定时器启动的初始值
	value.it_value.tv_sec = 0;
	value.it_value.tv_usec = 0;

	//定时器启动后的间隔时间值
	value.it_interval.tv_sec = 0;
	value.it_interval.tv_usec = 0;
	setitimer(ITIMER_REAL,&value,NULL);
}

//获取一个字符而 不回显
static int getch()
{
	struct termios tm_old;
	struct termios tm;
	tcgetattr(0, &tm_old);
	cfmakeraw(&tm);
	tcsetattr(0, 0, &tm);
	int ch = getchar();
	tcsetattr(0, 0, &tm_old);

	return ch;
}

#endif //WIN32-------------------------------------------

//信号注册函数
void catch_signal(int signo)
{
	//向下移动图形，一直到底部
	move_shape_down(num,mode,color);
	// signal(SIGALRM,catch_signal);
}

static void xyconsoletobox(int *a, int *b)
{
	*a = (*a - 12) / 2;
	*b = *b - 6;
}

//碰撞检测,检测方块是否碰撞到边界或其他方块
static int judge_shape(int n,int m,int a,int b)
{
	int xx, yy;
	int *sp = shape[n][m];

	xyconsoletobox(&a, &b);

	if(a < 0) return 1; //左边界
	if(a > 28 / 2 - (4 - sp[16])) return 1; //右边界
	if(b > 24 - (4 - sp[17])) return 1; //下边界

	/* 行 减sp[17]是为了避免图形到达边界判断matrix数组越界
	 *
	 *      [][]#
	 *      [][]#
	 *
	 * 假设如图#代表墙壁，田字方块到达右边界，
	 * 但是田字方块右边的方块不需要考虑也不可以考虑
	 * 这时matrix[b][a]中横坐标(a + 3) * 2会越界
	 */
	for(yy = 0; yy < 4 - sp[17]; yy++)
	{
		for(xx = 0; xx < 4 - sp[16]; xx++)  //列
		{
			if((sp[yy * 4 + xx] & matrix[b + yy][(a + xx) * 2]))
			{
				return 1;
			}
		}
	}
	return 0;
}



//保存方块棋盘信息，方块落地后，需将整个方块图形进行保存
static void store_shape()
{
	int i,  j;
	int a, b;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if (shape[num][mode][4 * i + j] == 1)
			{
				a = x + 2 * j;
				b = y + i;
				xyconsoletobox(&a, &b);
				matrix[b][a * 2] = 1;
				matrix[b][a * 2 + 1] = color;
			}
		}
	}
}



//生成新图形
static void new_shape()
{
	x = i_x;
	y = i_y;
	num = n_num;
	mode = n_mode;
	color = n_color;
	n_num = random()%(6-0+1)+0;
	n_mode = random()%(3-0+1)+0;
	n_color = random()%(36-30+1)+31;
}



//消行检测，当一行满时，进行消行操作
static void destroy_line()
{
	int i, j, k;
	int neederase;
	int lines = 0;
	for (i = 23; i >= 0; )
	{
		neederase = 1;
		for (j = 0; j < 28; j += 2)
		{
			if (matrix[i][j] == 0)
			{
				neederase = 0;
				break;
			}
		}
		if (neederase == 1)
		{
			lines++;
			for (j = i; j > 0; j--)
			{
				for (k = 0; k < 28; k++)
				{
					matrix[j][k] = matrix[j - 1][k];
				}
			}
		}
		else
			i--;
	}
	score += 10 * lines;
}



//判断游戏是否结束
static int is_over()
{
	int i;
	for (i = 5; i < 9; i++)
	{
		if (matrix[0][i * 2] == 1)
		{
			g_GameStatus = GAMEOVER;
			return 1;
		}
	}
	return 0;
}



//图形下落函数
static void move_shape_down()
{
	//首先判断，图形有没有触底
	if (judge_shape(num, mode, x, y + 1) == 1)		//触底
	{
		//保存现有图形
		store_shape();
		//消行
		destroy_line();
		//重新打印地图、分数
		print_matrix();
		print_score_level();
		//判断游戏是否结束
		if (is_over() == 1)
		{
			game_over();
			close_alarm();
		}
		//生成新图形
		eraser_shape(n_num, n_mode, n_x, n_y);
		new_shape();
		print_mode_shape(num, mode, x, y, color);
		print_mode_shape(n_num, n_mode, n_x, n_y, n_color);
		// close_alarm();
	}
	else									//移动后不会触底
	{
		//先清理原有图形
		eraser_shape(num, mode, x, y);
		y++;
		print_mode_shape(num, mode, x, y, color);
	}
}



static void fall_down()
{
	//直接落地
	int step = 1;
	for (step; judge_shape(num, mode, x, y + step) != 1; step++) ;
	step--;
	eraser_shape(num, mode, x, y);
	y += step;
	print_mode_shape(num, mode, x, y, color);
}



//方块左移
static void move_shape_left()
{
	//如果越界，返回
	//检测碰撞
	if (judge_shape(num, mode, x - 2, y) == 1)		//撞墙
	{
		return ;
	}
	//消除当前图形 左移 打印
	else
	{
		//先清理原有图形
		eraser_shape(num, mode, x, y);
		x -= 2;
		print_mode_shape(num, mode, x, y, color);
	}
}



//方块右移
static void move_shape_right()
{
	//如果越界，返回
	//检测碰撞
	if (judge_shape(num, mode, x + 2, y) == 1)		//撞墙
	{
		return ;
	}
	//消除当前图形 左移 打印
	else
	{
		//先清理原有图形
		eraser_shape(num, mode, x, y);
		x += 2;
		print_mode_shape(num, mode, x, y, color);
	}
}



//方块变形
static void change_shape()
{
	//获取下一个可变换的状态
	int tm = mode < 3 ? mode + 1 : 0;
	if(judge_shape(num, tm, x, y) == 1)
	{
		//变换产生碰撞
	}
	else
	{
		//擦除原来的形状
		eraser_shape(num, mode, x, y);
		//打印新形状
		print_mode_shape(num, tm, x, y, color);
		//修改当前形状为新形状
		mode = tm;
	}
}

void key_control()
{
	static int count;
	int ch;
	//q退出
	//回车直接到底
	//空格暂停
	//上 方块旋转
	//下 方块下移
	//左 方块左移
	//右 方块右移

#ifdef WIN32 //---------------Windows------------
	DWORD threadId;
	HANDLE tHandle = CreateThread(NULL, 0, ThreadProc, 0, 0, &threadId);
	
	while (1)
	{
		ch = getch();
		switch (ch)
		{
			case 72://KEY_UP:
				change_shape();
				break;
			case 80://KEY_DOWN:
				move_shape_down();
				break;
			case 75://KEY_LEFT:
				move_shape_left();
				break;
			case 77://KEY_RIGHT:
				move_shape_right();
				break;
			case 13://KEY_ENTER:
				fall_down();
				break;
			case 32://KEY_SPACE:
				// close_alarm();
				break;
			case 113://KEY_Q:
				clear();
				game_over();
				return;
				break;
			default:
				break;
		}
	}

#else //----------------Linux--------------------

	while (1)
	{
		ch = getch();

		switch (ch)
		{
			case 65://KEY_UP:
			{
				if (g_GameStatus == GAMEON)
					change_shape();
				break;
			}
			case 66://KEY_DOWN:
			{
				if (g_GameStatus == GAMEON)
					move_shape_down();
				break;
			}
			case 68://KEY_LEFT:
			{
				if (g_GameStatus == GAMEON)
					move_shape_left();
				break;
			}
			case 67://KEY_RIGHT:
			{
				if (g_GameStatus == GAMEON)
					move_shape_right();
				break;
			}
			case 13://KEY_ENTER:
			{
				if (g_GameStatus == GAMEON)
					fall_down();
				break;
			}
			case 32://KEY_SPACE:
			{
				if (g_GameStatus == GAMEON)
				{
					g_GameStatus = GAMEPAUSE;
					close_alarm();
				}
				else if (g_GameStatus == GAMEPAUSE)
				{
					g_GameStatus = GAMEON;
					alarm_us(tm);
				}
				// close_alarm();
				break;
			}
			case 113://KEY_Q:
			{
				game_over();
				clear();
				printf("\r\n");
				return;
				break;

			}
			default:
			{
				if (g_GameStatus == GAMESTOP)
				{
					start_game();
					continue;
				}
				break;
			}
		}
	}

#endif //WIN32-----------------------------------


}




