#ifndef _PRINT_H_
#define _PRINT_H_

extern int s_x;
extern int s_y;
extern int l_x;
extern int l_y;

extern int num,mode,color;
extern int n_num,n_mode,n_color;
extern int i_x,i_y;
extern int n_x,n_y;

void print_start_interface();
int getch();

void print_mode_shape(int n,int m,int x,int y,int c);
void print_next();
void eraser_shape(int,int,int,int,int);

#endif
