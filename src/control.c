#include "control.h"
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
//#include <termios.h>				//linux

//extern struct termios tm_old;		//linux
extern int num,mode,color;
extern int n_num,n_mode,n_color;
extern int i_x,i_y;
extern int x,y;
extern int s_x,s_y,l_x,l_y;

int tm = 800000;
extern void eraser_shape(int,int,int,int,int);
extern int shape[7][4][18];

int p_x = 60,p_y = 15;
int matrix[24][28] = {0};
int score,level;

#ifdef WIN32	//Windows 

#else	//Linux 

//΢�ʱ��,��ʱ��һ����������ÿ��һ��ʱ�䷢��SIGALRM�ź�
void alarm_us(int t)
{
	struct itimerval value;
	//��ʱ�������ĳ�ʼֵ
	value.it_value.tv_sec = 0;
	value.it_value.tv_usec = t;

	//��ʱ��������ļ��ʱ��ֵ
	value.it_interval.tv_sec = 0;
	value.it_interval.tv_usec = t;

	setitimer(ITIMER_REAL,&value,NULL);
}

//�ź�ע�ắ��
void catch_signal(int signo)
{
	//�����ƶ�ͼ�Σ�һֱ���ײ�
	move_shape_down(num,mode,color);
	signal(SIGALRM,catch_signal);
}

//�رն�ʱ��
void close_alarm()
{
	struct itimerval value;
	//��ʱ�������ĳ�ʼֵ
	value.it_value.tv_sec = 0;
	value.it_value.tv_usec = 0;

	//��ʱ��������ļ��ʱ��ֵ
	value.it_interval.tv_sec = 0;
	value.it_interval.tv_usec = 0;
	setitimer(ITIMER_REAL,&value,NULL);
}

#endif	//WIN32

//��ײ���,��ⷽ���Ƿ���ײ���߽����������
int judge_shape(int n,int m,int x,int y)
{
	int xx, yy;
	int *sp = shape[n][m];
	
	if(x < 0) return 1; //��߽� 
	if(x > 28 / 2 - sp[16]) return 1; //�ұ߽� 
	if(y > 24 - sp[17]) return 1; //�±߽� 
	
	/* �� ��sp[17]��Ϊ�˱���ͼ�ε���߽��ж�matrix����Խ�� 
	 *
	 *      [][]#
	 *      [][]#
	 *  
	 * ������ͼ#����ǽ�ڣ����ַ��鵽���ұ߽磬
	 * �������ַ����ұߵķ��鲻��Ҫ����Ҳ�����Կ���
	 * ��ʱmatrix[y][x]�к�����(x + 3) * 2��Խ��
	 */ 
	for(yy = 0; yy < 4 - sp[17]; yy++)  
	{
		for(xx = 0; xx < 4 - sp[16]; xx++)  //��
		{
			if((sp[yy * 4 + xx] & matrix[y + yy][(x + xx) * 2]))
			{
				return 1;
			}
		}
	}
	return 0;
}
//���淽��������Ϣ��������غ��轫��������ͼ�ν��б���
static void store_shape(int n,int m,int x,int y)
{

}
//��ʼ������
static void init_shape()
{
	//���������䵽�ײ�ʱ�������������µķ���
	//��ӡ���Ʒ���

}
//��ӡ����
static void print_matrix()
{

}
//��ӡ����������ȼ�
static void print_score_level()
{

}
//���м�⣬��һ�в���ʱ���������в���
static void destroy_line()
{
	//��⵽����
	//�����ۼ�
	//���´�ӡ��ͼ
}

extern void game_over();
//�ж���Ϸ�Ƿ����
int is_over()
{

}
//ͼ�����亯��
int move_shape_down(int n,int m,int c)
{
	//�����жϣ�ͼ����û�д���
	//��������ͼ��
	//����
	//�ƶ��󲻻ᴥ��
	//������ԭ��ͼ��

}

static void fall_down()
{
	//ֱ�����
}
//��������
static int move_shape_left(int n,int m,int a,int b,int c)
{
	//����������ƣ�ֱ�ӷ��� 
	if(judge_shape(n, m, x - 1, y)) return 0;
	
	//�������ƣ��ı������ 
	x--;
	
	
	
	//���Խ�磬����
	//�����ײ


	//������ǰͼ��
	//����
	//��ӡ


	return 0;
}
//��������
static int move_shape_right(int n,int m,int a,int b,int c)
{
	//���Խ�磬����
	//�����ײ


	//������ǰͼ��
	//����
	//��ӡ

	return 0;
}
//�������
static int change_shape(int n,int m,int c)
{
	//��ȡ��һ���ɱ任��״̬ 
	int tm = m < 3 ? m + 1 : 0;
	if(judge_shape(n, tm, x, y))  
	{
		//�任������ײ
		return 0;
	}
	else
	{
		//����ԭ������״ 
		eraser_shape(n, m, x, y, c);
		//��ӡ����״ 
		print_mode_shape(n, tm, x, y, c);
		//�޸ĵ�ǰ��״Ϊ����״ 
		mode = tm;
		return 1; 
	}
}

void game_over()
{
	//�̶��ص��ӡ��Ϸ��������

}

void key_control()
{
	static int count;
	int ch;
	//q�˳�
	//�س�ֱ�ӵ���
	//�ո���ͣ
	//�� ������ת
	//�� ��������
	//�� ��������
	//�� ��������

}
