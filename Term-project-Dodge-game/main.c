// 2022 Programming Lab

#define _CRT_SECURE_NO_WARNINGS

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"

typedef struct _Bullet {
    int bX;
    int bY;
    int bColor;         // 1 ~ 15 컬러코드
    int bOrient;        // 0 : 12시, 1 : 1~2시, 2 : 세시, ... , 6 : 9시, 7 : 10 ~ 11시 방향
} Bullet;

// 탄막들 정보 구조체
Bullet Bullet_info[TOTAL_BULLET];

int pX, pY;             // 플레이어 좌표
int bullet_num;         // 현재 탄막 개수

/*
* 방향(orientation)
*          0
*      7  ↑  1
*    6  ←  →  2
*      5  ↓  3
*         4
*/

// 탄막(총알) 생성
void CreateBullet() {
    int init_point, rand_color;

    rand_color = rand() % 3 + 5;            // 총알 컬러 랜덤 지정
    Bullet_info[bullet_num].bColor = rand_color;

    init_point = rand() % 4;                // 화면의 4분면 중 어디서 스폰할지

    switch (init_point) {
        // 제 1,2 사분면 (위)
        case 0:
            Bullet_info[bullet_num].bX = rand() % (WIDTH - 5) + 2; 
            Bullet_info[bullet_num].bY = 2;
            Bullet_info[bullet_num].bOrient = rand() % 3 + 3;
            break;
        // 제 2, 3 사분면 (좌)
        case 1:
            Bullet_info[bullet_num].bX = 2;
            Bullet_info[bullet_num].bY = rand() % (HEIGHT - 10) + 2;
            Bullet_info[bullet_num].bOrient = rand() % 3 + 1;
            break;
        // 제 3, 4 사분면 (아래)
        case 2:
            Bullet_info[bullet_num].bX = rand() % (WIDTH - 6) + 2;
            Bullet_info[bullet_num].bY = HEIGHT - 4;
            Bullet_info[bullet_num].bOrient = (rand() % 3 + 7) % 8; // { ([0, 1, 2] + 7) = [7, 8, 9] } % 8 = [7, 0, 1]
            break;
        // 제 4, 1 사분면 (우)
        case 3:
            Bullet_info[bullet_num].bX = WIDTH - 4;
            Bullet_info[bullet_num].bY = rand() % (HEIGHT - 10) + 2;
            Bullet_info[bullet_num].bOrient = rand() % 3 + 5;
            break;
    }
    bullet_num++;
}

void ClearBullet() {
    for (size_t i = 0; i < bullet_num - 1; i++) {
        bufferPrintXY(Bullet_info[i].bX, Bullet_info[i].bY, " ");
    }
}



void MoveBullet(int *px, int *py, int orient) {
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

/*
 Bullet_info[]의 n번째 총알 삭제(화면에서 지우는게 아님)
 = 뒤에서 앞으로 한칸씩 땡겨서 n번째 인덱스의 총알 제거
*/
void DelBullet(size_t n) {
    for (size_t i = n; i < bullet_num - 1; i++) {
        Bullet_info[i].bX = Bullet_info[i + 1].bX;
        Bullet_info[i].bY = Bullet_info[i + 1].bY;
        Bullet_info[i].bColor = Bullet_info[i + 1].bColor;
        Bullet_info[i].bOrient = Bullet_info[i + 1].bOrient;
    }
    bullet_num--;
}

void PrintBullet() {
    int newBullet;
    newBullet = 0;

    for (size_t i = 0; i < bullet_num; i++) {
        MoveBullet(&Bullet_info[i].bX, &Bullet_info[i].bY, Bullet_info[i].bOrient);     // 총알을 한 프레임 

        /* 화면 밖으로 나가면 탄막 배열에서 제거 */
        if (Bullet_info[i].bX < 3 || Bullet_info[i].bX > WIDTH - 3) {
            DelBullet(i);
            i--;
            newBullet++;
        } else if (Bullet_info[i].bY < 1 || Bullet_info[i].bY > HEIGHT - 3) {
            DelBullet(i);
            i--;
            newBullet++;
        }
    }

    /* 제거된 만큼 다시 생성 */
    for (size_t i = 0; i < newBullet; i++) {
        CreateBullet();
    }

    /* 실제로 탄막을 프린트하는 부분 */
    for (size_t i = 0; i < bullet_num; i++) {
        bufferSetColor(Bullet_info[i].bColor, BLACK);
        bufferPrintXY(Bullet_info[i].bX, Bullet_info[i].bY, "★");
        bufferSetColor(WHITE, BLACK);
    }
}

/* 플레이어 이동 : https://coding-factory.tistory.com/693 참고 */
void player1() {
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

    bufferSetColor(GREEN2, BLACK);
    bufferPrintXY(pX, pY, "Ω");
    bufferSetColor(WHITE, BLACK);
}

void timer_before_game() {
    setColor(WHITE, GREEN2);
    printXY(WIDTH / 2 - 8, HEIGHT / 2 - 1, "┌──────┐");
    printXY(WIDTH / 2 - 8, HEIGHT / 2,     "│ＴＨＲＥＥ！│");
    printXY(WIDTH / 2 - 8, HEIGHT / 2 + 1, "└──────┘");
    Sleep(1000);

    setColor(WHITE, CYAN1);
    printXY(WIDTH / 2 - 8, HEIGHT / 2 - 1, "┌──────┐");
    printXY(WIDTH / 2 - 8, HEIGHT / 2,     "│　ＴＷＯ！　│");
    printXY(WIDTH / 2 - 8, HEIGHT / 2 + 1, "└──────┘");
    Sleep(1000);

    setColor(WHITE, RED2);
    printXY(WIDTH / 2 - 8, HEIGHT / 2 - 1, "┌──────┐");
    printXY(WIDTH / 2 - 8, HEIGHT / 2,     "│　ＯＮＥ！　│");
    printXY(WIDTH / 2 - 8, HEIGHT / 2 + 1, "└──────┘");
    Sleep(1000);

    setColor(WHITE, BLACK);
    return;
}

void game() {
    pX = WIDTH / 2;
    pY = HEIGHT / 2;

    scr_init();

    while (1) {
        /* double buffer start >> */

        for (size_t i = 0; i < TOTAL_BULLET; i++) {
            CreateBullet();
        }

        while (1) {
            scr_switch();
            scr_clear();
            
            player1();

            ClearBullet();
            PrintBullet();

            Sleep(15);
        }
        /* >> double buffer end */
    }
    
}

int main() {
    int select;
    FILE *pF;
    Score scr[3] = { {"AAA", 1111}, {"BBB", 999}, {"CCC", 3333} };

    srand((unsigned int) time(NULL));

    pF = fopen(DATA, "r+b");
    if (pF == NULL) {
        pF = fopen(DATA, "w+b");
        if (pF == NULL) {
            printXY(0, HEIGHT - 1, "Can not open file. Exiting...");
            exit(1);
        }
    }

    fseek(pF, 0, SEEK_SET);
    for (size_t i = 0; i < 3; i++) {
        fwrite(&scr[i], sizeof(Score), 1, pF);
    }
    
    /* Main menu */

    init();
    draw_main_menu();

    select = draw_main_menu_sel();

    /*
     * Selection 
     * select = Q (0) : quit
     * select = S (2) : show score
     * select = F (1) : play game
     */

    switch (select) {
        case 0:
            exit(0);
        case 1:
            cls(WHITE, BLACK);
            timer_before_game();
            game();
            break;
        case 2:
            score_menu(pF);
            main();
            break;
    }

    fclose(pF);
    return 0;
}