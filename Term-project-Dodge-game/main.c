// 2022 Programming Lab

#define _CRT_SECURE_NO_WARNINGS

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"

#define WIDTH  104
#define HEIGHT 33
#define DATA   "score.bin"

typedef struct _Score {
    char name[4];
    int score;
} Score;

int Delay = 100; // 100 msec delay, �� ���� ���̸� �ӵ��� ��������.
int keep_moving = 1; // 1:����̵�, 0:��ĭ���̵�.

// Reset CMD color, hide cursor, set size
void init() {
    cls(WHITE, BLACK);
    removeCursor();
    setCmdSize(WIDTH, HEIGHT);
}

// Draw main menu
void draw_main_menu() {
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

// returns 0: Quit, 1: start game, 2: score
int draw_main_menu_sel() {
    int iSel;
    
    printXY(35, 19, "�� Selection : ");
    showCursor();

    while (1) {
        printXY(49, 19, " ");
        char cSel = getchar();

        if (cSel == 'Q') {
            printXY(38, 21, "                   ");
            printXY(38, 21, "Quit");
            gotoXY(0, HEIGHT - 1);
            return iSel = 0;
            // exit(0);
        } else if (cSel == 'F') {
            printXY(38, 21, "                   ");
            printXY(38, 21, "Start game");
            Sleep(1000);
            return iSel = 1;
            //break;
        } else if (cSel == 'S') {
            printXY(38, 21, "                   ");
            printXY(38, 21, "Show score");
            Sleep(1000);
            return iSel = 2;
            //break;
        } else {
            printXY(38, 21, "                   ");
            setColor(WHITE, RED2);
            printXY(38, 21, "Please enter again.");
            setColor(WHITE, BLACK);
            gotoXY(50, 19);
        }
    }
}

void score_menu() {
    char ch;
    cls(WHITE, BLACK);
    removeCursor();
    drawBox(0, 0, WIDTH - 2, HEIGHT - 2);

    while (1) {
        printXY(35, 5, "          <Score Board>          ");
        showCursor();
        gotoXY(35, 27);
        ch = getchar();
    }

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
    char buf[100];
    int select = 0;
    FILE *pF;

    init();

    draw_main_menu();


    /* Main menu selection */
    select = draw_main_menu_sel();

    // select = Q (0) : quit
    // select = S (2) : show score
    // select = F (1) : play game

    switch (select) {
        case 0:
            exit(0);
        case 1:
            // game();
            printXY(10, 10, "start game");
            cls(WHITE, BLACK);
            //drawBox(0, 0, WIDTH - 2, HEIGHT - 2);
            break;
        case 2:
            score_menu();
            break;
    }

    pF = fopen(DATA, "r+b");
    if (pF == NULL) {
        pF = fopen(DATA, "w+b");
        if (pF == NULL) {
            printXY(0, HEIGHT - 1, "Can not open file. Exiting...");
            exit(1);
        }
    }

    gotoXY(36, 10);

    char nick[3];
    int i;
    Score temp;
    Score scr[3] = { {"AAA", 1111}, {"BBB", 2222}, {"CCC", 3333} };

    fseek(pF, 0, SEEK_SET);
    for (i = 0; i < 3; i++) {
        fwrite(&scr[i], sizeof(Score), 1, pF);
    }

    gotoXY(36, 10);
    cls(WHITE, BLACK);

    fseek(pF, 0, SEEK_SET);
    for (i = 0; i < 3; i++) {
        fread(&temp, sizeof(Score), 1, pF);
        printf("[%02d]\t%s\t%d\n", i + 1, temp.name, temp.score);
    }

    fclose(pF);


    return 0;
}