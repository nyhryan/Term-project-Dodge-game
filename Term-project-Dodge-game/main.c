// 2022 Programming Lab

#define _CRT_SECURE_NO_WARNINGS

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"

#define DATA   "score.bin"
#define TOTAL_BULLET 50

typedef struct _Bullet {
    int bX;
    int bY;
    int bOrient;        // 0 : 12시, 1 : 1~2시, 2 : 세시, ... , 6 : 9시, 7 : 10 ~ 11시 방향
} Bullet;

// 탄막들 정보 구조체
Bullet Bullet_info[TOTAL_BULLET];

int Delay = 50;         // 100 msec delay, 이 값을 줄이면 속도가 빨라진다.
int keep_moving = 1;    // 1:계속이동, 0:한칸씩이동.

int pX, pY;             //플레이어 좌표


// 현재 탄막 개수
int bullet_num = 0;



// 탄막 생성
void CreateBullet() {
    int init_point = rand() % 4;         // 화면의 4분면 중 어디서 스폰할지

    switch (init_point) {
        // 제 1,2 사분면 (위)
        case 0:
            Bullet_info[bullet_num].bX = rand() % (WIDTH - 5) + 2; 
            //Bullet_info[bullet_num].bX = 20; 
            Bullet_info[bullet_num].bY = 2;
            Bullet_info[bullet_num].bOrient = rand() % 3 + 3;
            break;
        // 제 2, 3 사분면 (좌)
        case 1:
            Bullet_info[bullet_num].bX = 2;
            //Bullet_info[bullet_num].bY = 20;
            Bullet_info[bullet_num].bY = rand() % (HEIGHT - 10) + 2;
            Bullet_info[bullet_num].bOrient = rand() % 3 + 1;
            break;
        // 제 3, 4 사분면 (아래)
        case 2:
            Bullet_info[bullet_num].bX = rand() % (WIDTH - 6) + 2;
            //Bullet_info[bullet_num].bX = WIDTH - 4;
            Bullet_info[bullet_num].bY = HEIGHT - 4;
            Bullet_info[bullet_num].bOrient = (rand() % 3 + 7) % 8; // { ([0, 1, 2] + 7) = [7, 8, 9] } % 8 = [7, 0, 1]
            break;
        // 제 4, 1 사분면 (우)
        case 3:
            Bullet_info[bullet_num].bX = WIDTH - 4;
            //Bullet_info[bullet_num].bY = 20;
            Bullet_info[bullet_num].bY = rand() % (HEIGHT - 10) + 2;
            Bullet_info[bullet_num].bOrient = rand() % 3 + 5;
            break;
    }
    bullet_num++;
}

void CopyBullet(int i, int j) {
    Bullet_info[i].bX = Bullet_info[j].bX;
    Bullet_info[i].bY = Bullet_info[j].bY;
    Bullet_info[i].bOrient = Bullet_info[j].bOrient;
}

void ClearBullet() {
    int i;
    for (i = 0; i < bullet_num - 1; i++) {
        bufferPrintXY(Bullet_info[i].bX, Bullet_info[i].bY, " ");
    }
}

void MoveBullet(int *px, int *py, int orient) {
    switch (orient) {
        case 0:
            *py += 1;
            break;
        case 1:
            *px += 1;
            *py += 1;
            break;
        case 2:
            *px += 2;
            break;
        case 3:
            *px += 1;
            *py -= 1;
            break;
        case 4:
            *py -= 1;
            break;
        case 5:
            *px -= 1;
            *py -= 1;
            break;
        case 6:
            *px -= 2;
            break;
        case 7:
            *px -= 1;
            *py += 1;
            break;
    }
}

// Bullet_info[]의 n번째 총알 삭제 (화면에서 지우는게 아님)
void DelBullet(int n) {
    int i;
    for (i = n; i < bullet_num - 1; i++) {
        CopyBullet(i, i + 1);
    }
    bullet_num--;
}

void PrintBullet() {
    int i, newBullet = 0;
    //scr_clear();

    for (i = 0; i < bullet_num; i++) {
        MoveBullet(&Bullet_info[i].bX, &Bullet_info[i].bY, Bullet_info[i].bOrient);

        if (Bullet_info[i].bX < 3 || Bullet_info[i].bX > WIDTH - 3) {
            DelBullet(i);
            i--;
            newBullet++;
        } else if (Bullet_info[i].bY < 2 || Bullet_info[i].bY > HEIGHT - 3) {
            DelBullet(i);
            i--;
            newBullet++;
        }
    }

    for (i = 0; i < newBullet; i++) {
        CreateBullet();
    }

    for (i = 0; i < bullet_num; i++) {
        bufferPrintXY(Bullet_info[i].bX, Bullet_info[i].bY, "⊙");
        //printf("⊙");
    }
}

/* 플레이어 이동 : https://coding-factory.tistory.com/693 참고 */
void player1() {
    //scr_clear();

    bufferPrintXY(pX, pY, " ");
    if (GetAsyncKeyState(VK_LEFT) & 0x8000) { //왼쪽
        pX -= 2;
        if (pX <= 1) {
            pX = 2;
        }
    }
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000) { //오른쪽
        pX += 2;
        if (pX >= WIDTH - 3) {
            pX = WIDTH - 4;
        }
    }
    if (GetAsyncKeyState(VK_UP) & 0x8000) { //위
        pY--;
        if (pY <= 0) {
            pY = 1;
        }
    }
    if (GetAsyncKeyState(VK_DOWN) & 0x8000) { //아래
        pY++;
        if (pY >= HEIGHT - 2) {
            pY = HEIGHT - 3;
        }
    }

    //setColor(YELLOW2, BLACK);
    bufferPrintXY(pX, pY, "☆");
    //setColor(WHITE, BLACK);
}

void game() {
    int i;
    pX = WIDTH / 2;
    pY = HEIGHT / 2;
    //cls(WHITE, BLACK);
    scr_init();

    while (1) {
        /* double buffer start */


        for (i = 0; i < TOTAL_BULLET; i++) {
            CreateBullet();
        }

        //bufferDrawBox(0, 0, WIDTH - 2, HEIGHT - 2);
        //PrintBullet();
        //Sleep(13);
        while (1) {
            scr_switch();
            scr_clear();

            ClearBullet();

            //setColor(YELLOW2, BLACK);
            player1();
            //setColor(WHITE, BLACK);

            PrintBullet();
            Sleep(15);
        }

        //Sleep(100);
        /* double buffer end */
    }
    
}

int main() {
    char buf[100];
    int i;
    int select = 0;
    FILE *pF;
    Score scr[3] = { {"AAA", 1111}, {"BBB", 999}, {"CCC", 3333} };

    srand((unsigned int) time(NULL));

    pF = fopen(DATA, "r+b");
    if (pF == NULL) {
        printXY(0, HEIGHT - 1, "Can not open file. Exiting...");
        exit(1);
    }

    fseek(pF, 0, SEEK_SET);
    for (i = 0; i < 3; i++) {
        fwrite(&scr[i], sizeof(Score), 1, pF);
    }
    //scr_init();

    init();
    draw_main_menu();

    /* Main menu - selection */
    select = draw_main_menu_sel();
    removeCursor();

    // select = Q (0) : quit
    // select = S (2) : show score
    // select = F (1) : play game

    switch (select) {
        case 0:
            exit(0);
        case 1:
            game();

            break;
        case 2:
            score_menu(pF);
            main();
    }


    gotoXY(36, 10);

    gotoXY(36, 10);
    cls(WHITE, BLACK);

    //fseek(pF, 0, SEEK_SET);
    //for (i = 0; i < 3; i++) {
    //    fread(&temp, sizeof(Score), 1, pF);
    //    printf("[%02d]\t%s\t%d\n", i + 1, temp.name, temp.score);
    //}

    fclose(pF);


    return 0;
}