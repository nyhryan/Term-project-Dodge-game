// 2022 Programming Lab

#define _CRT_SECURE_NO_WARNINGS

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"

#define WIDTH  104
#define HEIGHT 33

int Delay = 100; // 100 msec delay, �� ���� ���̸� �ӵ��� ��������.
int keep_moving = 1; // 1:����̵�, 0:��ĭ���̵�.

// Reset CMD color, hide cursor, set size
void init() {
    cls(WHITE, BLACK);
    removeCursor();
    setCmdSize(WIDTH, HEIGHT);
}

// Draw main menu
void main_menu() {
    drawBox(0, 0, WIDTH - 2, HEIGHT - 2);
    printXY(2, HEIGHT - 3, "���������к� 2291012 ������");

    printXY(35, 10, "����������<Bullet  Hell>����������");
    printXY(35, 11, "��                              ��");
    printXY(35, 12, "��         - Start [F]          ��");
    printXY(35, 13, "��         - Score [S]          ��");
    printXY(35, 14, "��                              ��");
    printXY(35, 15, "��         - Quit  [Q]          ��");
    printXY(35, 16, "��                              ��");
    printXY(35, 17, "����������������������������������");
}

//void player1(unsigned char ch) {
//    static int oldx = 50, oldy = 10, newx = 50, newy = 10;
//    int move_flag = 0;
//    static int called = 0;
//    static unsigned char last_ch = 0;
//
//    if (called == 0) { // ���� ��ġ
//        removeCursor();
//        putstar(oldx, oldy, '��');
//        called = 1;
//    }
//    if (keep_moving && ch == 0)
//        ch = last_ch;
//    last_ch = ch;
//
//    switch (ch) {
//        case UP:
//            if (oldy > 0)
//                newy = oldy - 1;
//            else { // ���� �ε�ġ�� ������ �ݴ�� �̵�
//                newy = oldy + 1;
//                last_ch = DOWN;
//            }
//            move_flag = 1;
//            break;
//        case DOWN:
//            if (oldy < HEIGHT - 1)
//                newy = oldy + 1;
//            else {
//                newy = oldy - 1;
//                last_ch = UP;
//            }
//            move_flag = 1;
//            break;
//        case LEFT:
//            if (oldx > 0)
//                newx = oldx - 1;
//            else {
//                newx = oldx + 1;
//                last_ch = RIGHT;
//            }
//            move_flag = 1;
//            break;
//        case RIGHT:
//            if (oldx < WIDTH - 1)
//                newx = oldx + 1;
//            else {
//                newx = oldx - 1;
//                last_ch = LEFT;
//            }
//            move_flag = 1;
//            break;
//    }
//    if (move_flag) {
//        erasestar(oldx, oldy); // ������ ��ġ�� * �� �����
//        putstar(newx, newy, "*"); // ���ο� ��ġ���� * �� ǥ���Ѵ�.
//        oldx = newx; // ������ ��ġ�� ����Ѵ�.
//        oldy = newy;
//    }
//
//}

int main() {
    //int i;
    char select, buf[100];

    init();
    main_menu();

    printXY(35, 19, "�� Selection : ");
    showCursor();

    while (1) {
        printXY(49, 19, " ");
        scanf("%c", &select);

        if (select == 'Q') {
            printXY(35, 21, "                   ");
            printXY(35, 21, "Quit");
            gotoXY(0, HEIGHT - 1);
            exit(0);
        } else if (select == 'F') {
            printXY(35, 21, "                   ");
            printXY(35, 21, "Start game");
            break;
        } else if (select == 'S') {
            printXY(35, 21, "                   ");
            printXY(35, 21, "Show score");
            break;
        } else {
            printXY(35, 21, "                   ");
            setColor(WHITE, RED2);
            printXY(35, 21, "Please enter again.");
            setColor(WHITE, BLACK);
            gotoXY(50, 19);
        }
    }

    // select = S : show score
    // select = F : play game


    return 0;
}