#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include <conio.h>
#include <stdio.h>
#include <Windows.h>

#define BLANK '.'

#define WIDTH  104
#define HEIGHT 33

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

#define ESC 0x1b //  ESC 누르면 종료

#define SPECIAL1 0xe0 // 특수키는 0xe0 + key 값으로 구성된다.
#define SPECIAL2 0x00 // keypad 경우 0x00 + key 로 구성된다.

#define UP  0x48 // Up key는 0xe0 + 0x48 두개의 값이 들어온다.
#define DOWN 0x50
#define LEFT 0x4b
#define RIGHT 0x4d

#define UP2		'w'
#define DOWN2	's'
#define LEFT2	'a'
#define RIGHT2	'd'

typedef struct _Score {
    char name[4];
    int score;
} Score;

void scr_init();
void scr_switch();
void scr_clear();
void scr_release();

void init();
void removeCursor(void);
void showCursor(void);
void setCmdSize(int cols, int rows);
void gotoXY(int x, int y);
void bufferGotoXY(int x, int y);
void printscr(char *str);
void putstar(int x, int y, char ch);
void printXY(int x, int y, char *ch);
void bufferPrintXY(int x, int y, char *ch);
void erasestar(int x, int y);
void setColor(int fg_color, int bg_color);
void cls(int fg_color, int bg_color);
void drawBox(int x1, int y1, int x2, int y2);
void bufferDrawBox(int x1, int y1, int x2, int y2);
void draw_main_menu();
int draw_main_menu_sel();
void score_menu(FILE *pF);


#endif