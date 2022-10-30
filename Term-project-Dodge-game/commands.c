#define _CRT_SECURE_NO_WARNINGS

#include "commands.h"

// Ŀ���� �Ⱥ��̰� �Ѵ�
void removeCursor(void) {
    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
    curInfo.bVisible = 0;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

// Ŀ���� ���̰� �Ѵ�
void showCursor(void) {
    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
    curInfo.bVisible = 1;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void setCmdSize(int cols, int rows) {
    char cmd[100];
    sprintf(cmd, "mode con cols=%d lines=%d", cols, rows);
    system(cmd);
}

// ���� ���ϴ� ��ġ�� Ŀ�� �̵�
void gotoXY(int x, int y) {
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void putstar(int x, int y, char ch) {
    gotoXY(x, y);
    putchar(ch);
}

void printXY(int x, int y, char *ch) {
    gotoXY(x, y);
    printf("%s", ch);
}

void erasestar(int x, int y) {
    gotoXY(x, y);
    putchar(BLANK);
}

void setColor(int fg_color, int bg_color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fg_color | bg_color << 4);
}

// ȭ�� ������ ���ϴ� �������� �����Ѵ�.
void cls(int fg_color, int bg_color) {
    char cmd[100];
    system("cls");
    sprintf(cmd, "COLOR %x%x", bg_color, fg_color);
    system(cmd);
}

// box �׸��� �Լ�, ch ���ڿ��� (x1,y1) ~ (x2,y2) box�� �׸���.
void drawBox(int x1, int y1, int x2, int y2) {
    int x, y;

    printXY(x1, y1, "��");
    printXY(x2, y1, "��");
    printXY(x1, y2, "��");
    printXY(x2, y2, "��");

    // "��"
    for (x = x1 + 2; x < x2; x += 2) {
        printXY(x, y1, "��");
        printXY(x, y2, "��");
    }

    // "��"
    for (y = y1 + 1; y < y2; y++) {
        printXY(x1, y, "��");
        printXY(x2, y, "��");
    }
}