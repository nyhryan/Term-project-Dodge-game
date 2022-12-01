#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include <conio.h>
#include <stdio.h>
#include <Windows.h>

#define DATA "score.txt"

#define WIDTH  62
#define HEIGHT 44

// 색상 정의
#define BLACK	 0
#define BLUE1	 1
#define GREEN1	 2
#define CYAN1	 3
#define RED1	 4
#define MAGENTA1 5
#define YELLOW1	 6
#define GRAY1	 7
#define GRAY2	 8
#define BLUE2	 9
#define GREEN2	 10
#define CYAN2	 11
#define RED2	 12
#define MAGENTA2 13
#define YELLOW2	 14
#define WHITE	 15

typedef struct _Score {
    char name[4];
    int score;
} Score;

typedef struct _Bullet {
	int bX;
	int bY;
	int bColor;         // 1 ~ 15 컬러코드
	int bOrient;        // 0 : 12시, 1 : 1~2시, 2 : 세시, ... , 6 : 9시, 7 : 10 ~ 11시 방향
	int bSpeed;
	int bShape;
} Bullet;

typedef struct _Tracker_Bullet {
	int bX;
	int bY;
	int bColor;
} Tracker_Bullet;

typedef struct _Player {
	int pX;
	int pY;
	int life;
} Player;

void scr_init(void);
void scr_switch(void);
void scr_clear(void);
void scr_release(void);

void init(void);
void removeCursor(void);
void showCursor(void);
void setCmdSize(int cols, int rows);
void gotoXY(int x, int y);
void bufferGotoXY(int x, int y);
void printscr(char *str);
void printXY(int x, int y, char *ch);
void bufferPrintXY(int x, int y, char *ch);
void setColor(int fg_color, int bg_color);
void bufferSetColor(int fg_color, int bg_color);
void cls(int fg_color, int bg_color);
void drawBox(int x1, int y1, int x2, int y2);
void bufferDrawBox(int x1, int y1, int x2, int y2);
void draw_main_menu(void);
int draw_main_menu_sel(void);
int compare(const void* a, const void* b);
void score_menu(FILE *pF);

void timer_before_game(void);


#endif