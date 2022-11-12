// 2022 Programming Lab

#define _CRT_SECURE_NO_WARNINGS

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"

#define TOTAL_TRACKER_BULLET 5

typedef struct _Bullet {
    int bX;
    int bY;
    int bColor;         // 1 ~ 15 �÷��ڵ�
    int bOrient;        // 0 : 12��, 1 : 1~2��, 2 : ����, ... , 6 : 9��, 7 : 10 ~ 11�� ����
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

// ź���� ���� ����ü
Bullet bullet_arr[TOTAL_BULLET];

/* �߰��ϴ� �Ѿ� */
Tracker_Bullet bullet_tracker_arr[TOTAL_TRACKER_BULLET];

Player p1;

int bullet_num;         // ���� ź�� ����
int bullet_tracker_num;

/*
* ����(orientation)
*          0
*      7  ��  1
*    6  ��  ��  2
*      5  ��  3
*         4
*/

// ź��(�Ѿ�) ����
void bullet_create(void) {
    int init_point, rand_color;

    rand_color = rand() % 3 + 5;            // �Ѿ� �÷� ���� ����
    bullet_arr[bullet_num].bColor = rand_color;

    init_point = rand() % 4;                // ȭ���� 4�и� �� ��� ��������

    switch (init_point) {
        // �� 1,2 ��и� (��)
        case 0:
            bullet_arr[bullet_num].bX = rand() % (WIDTH - 5) + 2;
            bullet_arr[bullet_num].bY = 2;
            bullet_arr[bullet_num].bOrient = rand() % 3 + 3;
            break;
            // �� 2, 3 ��и� (��)
        case 1:
            bullet_arr[bullet_num].bX = 2;
            bullet_arr[bullet_num].bY = rand() % (HEIGHT - 10) + 2;
            bullet_arr[bullet_num].bOrient = rand() % 3 + 1;
            break;
            // �� 3, 4 ��и� (�Ʒ�)
        case 2:
            bullet_arr[bullet_num].bX = rand() % (WIDTH - 6) + 2;
            bullet_arr[bullet_num].bY = HEIGHT - 4;
            bullet_arr[bullet_num].bOrient = (rand() % 3 + 7) % 8; // { ([0, 1, 2] + 7) = [7, 8, 9] } % 8 = [7, 0, 1]
            break;
            // �� 4, 1 ��и� (��)
        case 3:
            bullet_arr[bullet_num].bX = WIDTH - 4;
            bullet_arr[bullet_num].bY = rand() % (HEIGHT - 10) + 2;
            bullet_arr[bullet_num].bOrient = rand() % 3 + 5;
            break;
    }
    bullet_num++;
}

/* �߰��ϴ� �Ѿ�*/
void bullet_tracker_create(void) {
    int rand_pos = rand() % 4;
    switch (rand_pos) {
        case 0:
            bullet_tracker_arr[bullet_tracker_num].bX = 4;
            bullet_tracker_arr[bullet_tracker_num].bY = 4;
            break;
        case 1:
            bullet_tracker_arr[bullet_tracker_num].bX = WIDTH - 4;
            bullet_tracker_arr[bullet_tracker_num].bY = 4;
            break;
        case 2:
            bullet_tracker_arr[bullet_tracker_num].bX = WIDTH - 4;
            bullet_tracker_arr[bullet_tracker_num].bY = HEIGHT - 4;
            break;
        case 3:
            bullet_tracker_arr[bullet_tracker_num].bX = 4;
            bullet_tracker_arr[bullet_tracker_num].bY = HEIGHT - 4;
            break;
    }

    bullet_tracker_arr[bullet_tracker_num].bColor = RED2;

    ++bullet_tracker_num;
}

void bullet_clear(void) {
    for (int i = 0; i < bullet_num - 1; i++) {
        bufferPrintXY(bullet_arr[i].bX, bullet_arr[i].bY, " ");
    }
}

void bullet_tracker_clear(int diff) {
    for (int i = 0; i < bullet_tracker_num - 1 - diff; i++) {
        bufferPrintXY(bullet_tracker_arr[i].bX, bullet_tracker_arr[i].bY, " ");
    }
}

void bullet_move(int* px, int* py, int orient) {
    switch (orient) {
        case 0:
            *py -= 1;
            break;
        case 1:
            *px += 2;
            *py -= 1;
            break;
        case 2:
            *px += 2;
            break;
        case 3:
            *px += 2;
            *py += 1;
            break;
        case 4:
            *py += 1;
            break;
        case 5:
            *px -= 2;
            *py += 1;
            break;
        case 6:
            *px -= 2;
            break;
        case 7:
            *px -= 2;
            *py -= 1;
            break;
    }
}

void bullet_tracker_move(int* bx, int* by) {
    int dx, dy;
    dx = *bx - p1.pX;
    dy = *by - p1.pY;

    if (dx < 0 && dy < 0) {
        *bx += 2;
        *by += 1;
    } else if (dx > 0 && dy < 0) {
        *bx -= 2;
        *by += 1;
    } else if (dx < 0 && dy > 0) {
        *bx += 2;
        *by -= 1;
    } else if (dx > 0 && dy > 0) {
        *bx -= 2;
        *by -= 1;
    } else if (dx < 0 && dy == 0) {
        *bx += 2;
    } else if (dx > 0 && dy == 0) {
        *bx -= 2;
    } else if (dx == 0 && dy < 0) {
        *by += 1;
    } else if (dx == 0 && dy > 0) {
        *by -= 1;
    }
}

/*
 bullet_arr[]�� n��° �Ѿ� ����(ȭ�鿡�� ����°� �ƴ�)
 = �ڿ��� ������ ��ĭ�� ���ܼ� n��° �ε����� �Ѿ� ����
*/
void bullet_delete(int n) {
    for (int i = n; i < bullet_num - 1; i++) {
        bullet_arr[i].bX = bullet_arr[i + 1].bX;
        bullet_arr[i].bY = bullet_arr[i + 1].bY;
        bullet_arr[i].bColor = bullet_arr[i + 1].bColor;
        bullet_arr[i].bOrient = bullet_arr[i + 1].bOrient;
    }
    bullet_num--;
}

void bullet_tracker_delete(int n) {
    for (int i = n; i < bullet_tracker_num - 1; i++) {
        bullet_tracker_arr[i].bX = bullet_tracker_arr[i + 1].bX;
        bullet_tracker_arr[i].bY = bullet_tracker_arr[i + 1].bY;
        bullet_tracker_arr[i].bColor = bullet_tracker_arr[i + 1].bColor;
        //bullet_tracker_arr[i].bOrient = bullet_tracker_arr[i + 1].bOrient;
    }
    bullet_tracker_num--;
}

void bullet_print() {
    int newBullet;
    newBullet = 0;

    for (int i = 0; i < bullet_num; i++) {
        bullet_move(&bullet_arr[i].bX, &bullet_arr[i].bY, bullet_arr[i].bOrient);     // �Ѿ��� �� ������ 

        /* ȭ�� ������ ������ ź�� �迭���� ���� */
        if (bullet_arr[i].bX < 3 || bullet_arr[i].bX > WIDTH - 3) {
            bullet_delete(i);
            i--;
            newBullet++;
        } else if (bullet_arr[i].bY < 1 || bullet_arr[i].bY > HEIGHT - 3) {
            bullet_delete(i);
            i--;
            newBullet++;
        }
    }

    /* ���ŵ� ��ŭ �ٽ� ���� */
    for (int i = 0; i < newBullet; i++) {
        bullet_create();
    }

    /* ������ ź���� ����Ʈ�ϴ� �κ� */
    for (int i = 0; i < bullet_num; i++) {
        bufferSetColor(bullet_arr[i].bColor, BLACK);
        bufferPrintXY(bullet_arr[i].bX, bullet_arr[i].bY, "��");
        bufferSetColor(WHITE, BLACK);
    }
}

void bullet_tracker_print(int frame_count, int diff) {
    int newBullet;
    newBullet = 0;

    for (int i = 0; i < bullet_tracker_num - diff; i++) {
        // �Ѿ��� �� ������ 
        if (frame_count % 5 == 0) {
            bullet_tracker_move(&bullet_tracker_arr[i].bX, &bullet_tracker_arr[i].bY);
        }

        /* ȭ�� ������ ������ ź�� �迭���� ���� */
        if (bullet_tracker_arr[i].bX < 3 || bullet_tracker_arr[i].bX > WIDTH - 3) {
            bullet_tracker_delete(i);
            i--;
            newBullet++;
        } else if (bullet_tracker_arr[i].bY < 1 || bullet_tracker_arr[i].bY > HEIGHT - 3) {
            bullet_tracker_delete(i);
            i--;
            newBullet++;
        }
    }

    /* ���ŵ� ��ŭ �ٽ� ���� */
    for (int i = 0; i < newBullet; i++) {
        bullet_tracker_create();
    }

    /* ������ ź���� ����Ʈ�ϴ� �κ� */
    for (int i = 0; i < bullet_tracker_num - diff; i++) {
        bufferSetColor(bullet_tracker_arr[i].bColor, BLACK);
        bufferPrintXY(bullet_tracker_arr[i].bX, bullet_tracker_arr[i].bY, "��");
        bufferSetColor(WHITE, BLACK);
    }
}

/* �÷��̾� �̵� : https://coding-factory.tistory.com/693 ���� */
void player1() {
    bufferPrintXY(p1.pX, p1.pY, " ");

    if (GetAsyncKeyState(VK_LEFT) & 0x8000) { //����
        p1.pX -= 2;
        if (p1.pX <= 1) {
            p1.pX = 2;
        }
    }
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000) { //������
        p1.pX += 2;
        if (p1.pX >= WIDTH - 3) {
            p1.pX = WIDTH - 4;
        }
    }
    if (GetAsyncKeyState(VK_UP) & 0x8000) { //��
        p1.pY--;
        if (p1.pY <= 0) {
            p1.pY = 1;
        }
    }
    if (GetAsyncKeyState(VK_DOWN) & 0x8000) { //�Ʒ�
        p1.pY++;
        if (p1.pY >= HEIGHT - 2) {
            p1.pY = HEIGHT - 3;
        }
    }

    bufferSetColor(GREEN2, BLACK);
    bufferPrintXY(p1.pX, p1.pY, "��");
    bufferSetColor(WHITE, BLACK);
}

void timer_before_game() {
    setColor(WHITE, GREEN2);
    printXY(WIDTH / 2 - 8, HEIGHT / 2 - 1, "����������������");
    printXY(WIDTH / 2 - 8, HEIGHT / 2, "���ԣȣңţţ���");
    printXY(WIDTH / 2 - 8, HEIGHT / 2 + 1, "����������������");
    Sleep(1000);

    setColor(WHITE, CYAN1);
    printXY(WIDTH / 2 - 8, HEIGHT / 2 - 1, "����������������");
    printXY(WIDTH / 2 - 8, HEIGHT / 2, "�����ԣףϣ�����");
    printXY(WIDTH / 2 - 8, HEIGHT / 2 + 1, "����������������");
    Sleep(1000);

    setColor(WHITE, RED2);
    printXY(WIDTH / 2 - 8, HEIGHT / 2 - 1, "����������������");
    printXY(WIDTH / 2 - 8, HEIGHT / 2, "�����ϣΣţ�����");
    printXY(WIDTH / 2 - 8, HEIGHT / 2 + 1, "����������������");
    Sleep(1000);

    setColor(WHITE, BLACK);
    return;
}

unsigned long print_score(const clock_t start) {
    char time[WIDTH];
    char temp[WIDTH];

    clock_t now, delta;

    now = clock();
    delta = now - start;

    bufferSetColor(BLACK, GRAY1);

    for (int i = 0; i < WIDTH; i++) {
        bufferPrintXY(i, HEIGHT - 2, " ");
    }

    sprintf(time, "Score: %d", delta);
    bufferPrintXY(WIDTH / 2 - 16, HEIGHT - 2, time);

    if (p1.life == 3) {
        sprintf(temp, "Life: ������");
    } else if (p1.life == 2) {
        sprintf(temp, "Life: ������");
    } else if (p1.life == 1) {
        sprintf(temp, "Life: ������");
    } else {
        sprintf(temp, "��");
    }

    bufferPrintXY(WIDTH / 2, HEIGHT - 2, temp);

    bufferSetColor(WHITE, BLACK);

    return (unsigned long) delta;
}

void game(Score* result) {
    int frame_count, difficulty;
    clock_t start;
    unsigned long current_delta;

    difficulty = TOTAL_TRACKER_BULLET;
    p1.pX = WIDTH / 2;
    p1.pY = HEIGHT / 2;
    p1.life = 3;

    for (int i = 0; i < TOTAL_BULLET; i++) {
        bullet_create();
    }

    for (int i = 0; i < TOTAL_TRACKER_BULLET; i++) {
        bullet_tracker_create();
    }

    start = clock();
    frame_count = 0;

    /* double buffer start >> */
    scr_init();
    while (1) {
        scr_switch();
        scr_clear();

        current_delta = print_score(start);

        player1();

        bullet_clear();
        bullet_print();

        if (current_delta > 1000) {
            if (difficulty > 0 && (frame_count % 500 == 0)) {
                --difficulty;
            }
            bullet_tracker_clear(difficulty);
            bullet_tracker_print(frame_count, difficulty);
        }

        for (int i = 0; i < TOTAL_BULLET; i++) {
            if ((p1.pX == bullet_arr[i].bX) && (p1.pY == bullet_arr[i].bY) ||
                (p1.pX == bullet_tracker_arr[i].bX) && (p1.pY == bullet_tracker_arr[i].bY)) {
                if (p1.life > 0) {
                    --p1.life;
                }
            }
        }

        if (p1.life == 0) {
            break;
        } else {
            ++frame_count;
            Sleep(15);
        }
    }
    scr_clear();
    scr_release();

    cls(WHITE, BLACK);
    printXY(WIDTH / 4, HEIGHT / 2, "YOU DIED");

    gotoXY(WIDTH / 4 + 26, 22);
    showCursor();
    scanf(" %s", &result->name);

    result->score = (int) current_delta;
    /* >> double buffer end */
}

int main() {
    FILE* pF;
    //Score scr[3] = { {"AAA", 1111}, {"BBB", 999}, {"CCC", 3333} };

    pF = fopen(DATA, "r+b");
    if (pF == NULL) {
        pF = fopen(DATA, "w+b");
        if (pF == NULL) {
            printXY(0, HEIGHT - 1, "Can not open file. Exiting...");
            exit(1);
        }
    }

    /* Main menu */
    while (1) {
        Score temp;
        int select;

        srand((unsigned int) time(NULL));

        init();
        draw_main_menu();

        select = draw_main_menu_sel();

        switch (select) {
            case 0:
                fclose(pF);
                exit(0);
            case 1:
                cls(WHITE, BLACK);
                timer_before_game();

                game(&temp);

                cls(WHITE, BLACK);
                printXY(WIDTH / 4, HEIGHT / 2, temp.name);

                fseek(pF, 0, SEEK_END);
                fwrite(&temp, sizeof(Score), 1, pF);

                break;
            case 2:
                score_menu(pF);
                break;
        }
    }
    return 0;
}