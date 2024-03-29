﻿#define _CRT_SECURE_NO_WARNINGS

#include "commands.h"

int hidden_index;
HANDLE scr_handle[2];

void scr_init(void) {
    CONSOLE_CURSOR_INFO cci;

    // 화면 버퍼 2개 생성
    scr_handle[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    scr_handle[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

    // 커서를 숨긴다
    cci.dwSize = 1;
    cci.bVisible = FALSE;
    SetConsoleCursorInfo(scr_handle[0], &cci);
    SetConsoleCursorInfo(scr_handle[1], &cci);

    // 0번화면이 디폴트
    SetConsoleActiveScreenBuffer(scr_handle[0]);
    hidden_index = 1;   // 1번화면이 hidden_screen
}

void scr_switch(void) {
    SetConsoleActiveScreenBuffer(scr_handle[hidden_index]);
    hidden_index = !hidden_index;   // hidden 0 <-> 1 toggle
}

void scr_clear(void) {
    COORD Coor = { 0, 0 };
    DWORD dw;
    FillConsoleOutputCharacter(scr_handle[hidden_index], ' ', WIDTH * 2 * HEIGHT, Coor, &dw);
}

void scr_release(void) {
    CloseHandle(scr_handle[0]);
    CloseHandle(scr_handle[1]);
}

// Reset CMD color, hide cursor, set size
void init(void) {
    cls(WHITE, BLACK);
    removeCursor();
    setCmdSize(WIDTH, HEIGHT);
}

// 커서를 안보이게 한다
void removeCursor(void) {
    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
    curInfo.bVisible = 0;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

// 커서를 보이게 한다
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

// 내가 원하는 위치로 커서 이동
void gotoXY(int x, int y) {
    COORD pos = { x, y };
     SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void bufferGotoXY(int x, int y) {
    COORD pos = { x, y };

    // hidden screen에 gotoxy
    SetConsoleCursorPosition(scr_handle[hidden_index], pos);

    // SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void printscr(char *str) {
    DWORD dw;

    // hidden screen에 gotoxy 되었다고 가정하고 print
    WriteFile(scr_handle[hidden_index], str, (DWORD) strlen(str), &dw, NULL);
}

void printXY(int x, int y, char *ch) {
    //DWORD dw;
    COORD pos = { x, y };

    // hidden screen에 gotoXY + print
    //SetConsoleCursorPosition(scr_handle[hidden_index], pos);
    //WriteFile(scr_handle[hidden_index], ch, strlen(ch), &dw, NULL);

    gotoXY(x, y);
    printf("%s", ch);
}

void bufferPrintXY(int x, int y, char *ch) {
    DWORD dw;
    COORD pos = { x, y };

    // hidden screen에 gotoXY + print
    SetConsoleCursorPosition(scr_handle[hidden_index], pos);
    WriteFile(scr_handle[hidden_index], ch, strlen(ch), &dw, NULL);

    //gotoXY(x, y);
    //printf("%s", ch);
}

void setColor(int fg_color, int bg_color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fg_color | bg_color << 4);
}

void bufferSetColor(int fg_color, int bg_color) {
    SetConsoleTextAttribute(scr_handle[hidden_index], fg_color | bg_color << 4);
}

// 화면 지우기고 원하는 배경색으로 설정한다.
void cls(int fg_color, int bg_color) {
    char cmd[100];
    system("cls");
    sprintf(cmd, "COLOR %x%x", bg_color, fg_color);
    system(cmd);
}

// box 그리기 함수, ch 문자열로 (x1,y1) ~ (x2,y2) box를 그린다.
void drawBox(int x1, int y1, int x2, int y2) {
    int x, y;

    printXY(x1, y1, "┌");
    printXY(x2, y1, "┐");
    printXY(x1, y2, "└");
    printXY(x2, y2, "┘");

    // "─"
    for (x = x1 + 2; x < x2; x += 2) {
        printXY(x, y1, "─");
        printXY(x, y2, "─");
    }

    // "│"
    for (y = y1 + 1; y < y2; y++) {
        printXY(x1, y, "│");
        printXY(x2, y, "│");
    }
}

// Used for double buffered output
void bufferDrawBox(int x1, int y1, int x2, int y2) {
    int x, y;

    bufferPrintXY(x1, y1, "┌");
    bufferPrintXY(x2, y1, "┐");
    bufferPrintXY(x1, y2, "└");
    bufferPrintXY(x2, y2, "┘");

    // "─"
    for (x = x1 + 2; x < x2; x += 2) {
        bufferPrintXY(x, y1, "─");
        bufferPrintXY(x, y2, "─");
    }

    // "│"
    for (y = y1 + 1; y < y2; y++) {
        bufferPrintXY(x1, y, "│");
        bufferPrintXY(x2, y, "│");
    }


}

// Draw main menu
void draw_main_menu(void) {
    int logo_x, box_x, pos_x;
    int logo[7][30] = {
        { 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1 },
        { 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0 },
        { 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 },
        { 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1 },
        { 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0 },
        { 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0 },
        { 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1 }
    };

    logo_x = (WIDTH - 60) / 2 + 1;
    
    setColor(WHITE, BLACK);

    //drawBox(0, 0, WIDTH - 2, HEIGHT - 2);
    printXY(2, HEIGHT - 3, "상상력인재학부 2291012 남윤혁");

    pos_x = 0;
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 30; j++) {
            pos_x = j * 2;
            if (logo[i][j]) {
                if (i < 2) {
                    setColor(CYAN2, CYAN2);
                } else if (i < 4) {
                    setColor(CYAN1, CYAN1);
                } else if (i < 6) {
                    setColor(BLUE2, BLUE2);
                } else {
                    setColor(BLUE1, BLUE1);
                }
                printXY(logo_x + pos_x, i + 2, "■");

            } else {
                setColor(BLACK, BLACK);
                printXY(logo_x + pos_x, i + 2, "■");
            }
        }
    }
    

    setColor(WHITE, BLACK);
    box_x = (WIDTH - 35) / 2;
    printXY(box_x, 10, "┌───────────────┐");
    printXY(box_x, 11, "│                              │");
    printXY(box_x, 12, "│         - Start [F]          │");
    printXY(box_x, 13, "│         - Score [S]          │");
    printXY(box_x, 14, "│                              │");
    printXY(box_x, 15, "│         - Quit  [Q]          │");
    printXY(box_x, 16, "│                              │");
    printXY(box_x, 17, "└───────────────┘");
}

// returns 0: Quit, 1: start game, 2: score
void draw_main_menu_sel(int* select) {
    int pos_x;
    char cSel;

    pos_x = (WIDTH - 35) / 2;
    printXY(pos_x, 18, "☞ Selection :  ");
    showCursor();

    while (1) {
        printXY(pos_x + 15, 18, "       ");
        gotoXY(pos_x + 15, 18);
        cSel = getch();
        putchar(cSel);
        removeCursor();
        if (cSel == 'Q') {
            printXY(pos_x, 20, "               ");
            setColor(BLACK, RED2);
            printXY(pos_x, 20, " > Quit ");
            setColor(WHITE, BLACK);
            //printXY(47, 20, "              ");
            gotoXY(0, HEIGHT - 1);
            *select = 0;
            break;
        } else if (cSel == 'F') {
            printXY(pos_x, 20, "               ");
            setColor(BLACK, CYAN2);
            printXY(pos_x, 20, " > Start game ");
            setColor(WHITE, BLACK);
            Sleep(1000);
            *select = 1;
            break;
        } else if (cSel == 'S') {
            printXY(pos_x, 20, "        ");
            setColor(BLACK, YELLOW2);
            printXY(pos_x, 20, " > Show score ");
            setColor(WHITE, BLACK);
            Sleep(1000);
            *select = 2;
            break;
        }
    }
}

int compare(const void* a, const void* b) {
    Score* pa = (Score*) a;
    Score* pb = (Score*) b;

    if (pa->score < pb->score) {
        return 1;
    } else if (pa->score > pb->score) {
        return -1;
    } else {
        return 0;
    }
}

// read, draws score
void score_menu(FILE *pF) {
    int pos_x, logo_x;
    int score_logo[7][41] = {
        { 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1 },
        { 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0 },
        { 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1 },
        { 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0 },
        { 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0 },
        { 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1 }
    };
    char cSel, output[50];
    Score temp[10] = { 0, };

    logo_x = (WIDTH - 41) / 2;

    cls(WHITE, BLACK);

    drawBox(0, 0, WIDTH - 2, HEIGHT - 2);

    fseek(pF, 0, SEEK_SET);
    for (int i = 0; i < 10; i++) {
        fread(&temp[i], sizeof(Score), 1, pF);
    }

    qsort(temp, 10, sizeof(Score), compare);

    while (1) {
        for (int i = 0; i < 7; i++) {
            for (int j = 0; j < 41; j++) {
                pos_x = j * 1;
                if (score_logo[i][j]) {
                    if (i < 2) {
                        setColor(CYAN2, CYAN2);
                    } else if (i < 4) {
                        setColor(CYAN1, CYAN1);
                    } else if (i < 6) {
                        setColor(BLUE2, BLUE2);
                    } else {
                        setColor(BLUE1, BLUE1);
                    }
                    printXY(logo_x + pos_x, i + 1, "a");
                } else {
                    setColor(BLACK, BLACK);
                    printXY(logo_x + pos_x, i + 1, "a");
                }
            }
        }


        setColor(WHITE, BLACK);
        pos_x = (WIDTH - 35) / 2;

        printXY(pos_x, 9, "┌────<Score  Board>────┐");
        printXY(pos_x, 10, "│                              │");
        for (int i = 0; i < 10; i++) {
            sprintf(output, "│      [%02d] %3s %8d       │\n",
                i + 1, temp[i].name, temp[i].score);
            printXY(pos_x, 11 + i, output);
        }
        printXY(pos_x, 21, "│                              │");
        printXY(pos_x, 22, "└───────────────┘");

        printXY(pos_x + 3, 23, "> Return to main [R] :  ");
        showCursor();
        gotoXY(pos_x + 26, 23);
        //scanf("%c", &cSel);
        cSel = getch();
        putchar(cSel);

        if (cSel == 'R') {
            printXY(pos_x + 3, 25, "                   ");
            setColor(BLACK, YELLOW2);
            printXY(pos_x + 3, 25, "> Returning...");
            setColor(WHITE, BLACK);
            Sleep(1000);
            return;
        }
    }

}

void timer_before_game(void) {
    setColor(WHITE, GREEN2);
    printXY(WIDTH / 2 - 8, HEIGHT / 2 - 1, "┌──────┐");
    printXY(WIDTH / 2 - 8, HEIGHT / 2, "│ＴＨＲＥＥ！│");
    printXY(WIDTH / 2 - 8, HEIGHT / 2 + 1, "└──────┘");
    Sleep(1000);

    setColor(WHITE, CYAN1);
    printXY(WIDTH / 2 - 8, HEIGHT / 2 - 1, "┌──────┐");
    printXY(WIDTH / 2 - 8, HEIGHT / 2, "│　ＴＷＯ！　│");
    printXY(WIDTH / 2 - 8, HEIGHT / 2 + 1, "└──────┘");
    Sleep(1000);

    setColor(WHITE, RED2);
    printXY(WIDTH / 2 - 8, HEIGHT / 2 - 1, "┌──────┐");
    printXY(WIDTH / 2 - 8, HEIGHT / 2, "│　ＯＮＥ！　│");
    printXY(WIDTH / 2 - 8, HEIGHT / 2 + 1, "└──────┘");
    Sleep(1000);

    setColor(WHITE, BLACK);
    return;
}