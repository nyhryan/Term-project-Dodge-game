#define _CRT_SECURE_NO_WARNINGS

#include "commands.h"

int hidden_index;
HANDLE scr_handle[2];

void scr_init() {
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

void scr_switch() {
    SetConsoleActiveScreenBuffer(scr_handle[hidden_index]);
    hidden_index = !hidden_index;   // hidden 0 <-> 1 toggle
}

void scr_clear() {
    COORD Coor = { 0, 0 };
    DWORD dw;

    FillConsoleOutputCharacter(scr_handle[hidden_index], ' ', WIDTH * 2 * HEIGHT, Coor, &dw);
}

void scr_release() {
    CloseHandle(scr_handle[0]);
    CloseHandle(scr_handle[1]);
}

// Reset CMD color, hide cursor, set size
void init() {
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
    DWORD dw;

    // hidden screen에 gotoxy
    SetConsoleCursorPosition(scr_handle[hidden_index], pos);

    // SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void printscr(char *str) {
    DWORD dw;

    // hidden screen에 gotoxy 되었다고 가정하고 print
    WriteFile(scr_handle[hidden_index], str, strlen(str), &dw, NULL);
}

void putstar(int x, int y, char ch) {
    gotoXY(x, y);
    putchar(ch);
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

void erasestar(int x, int y) {
    gotoXY(x, y);
    putchar(BLANK);
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
void draw_main_menu() {
    drawBox(0, 0, WIDTH - 2, HEIGHT - 2);
    printXY(2, HEIGHT - 3, "상상력인재학부 2291012 남윤혁");

    printXY(WIDTH / 4, 10, "┌────<Bullet  Hell>────┐");
    printXY(WIDTH / 4, 11, "│                              │");
    printXY(WIDTH / 4, 12, "│         - Start [F]          │");
    printXY(WIDTH / 4, 13, "│         - Score [S]          │");
    printXY(WIDTH / 4, 14, "│                              │");
    printXY(WIDTH / 4, 15, "│         - Quit  [Q]          │");
    printXY(WIDTH / 4, 16, "│                              │");
    printXY(WIDTH / 4, 17, "└───────────────┘");
}

// returns 0: Quit, 1: start game, 2: score
int draw_main_menu_sel() {
    int iSel;
    char cSel;

    printXY(WIDTH / 4, 19, "☞ Selection :  ");
    showCursor();

    while (1) {
        printXY(WIDTH / 4 + 15, 19, "       ");
        gotoXY(WIDTH / 4 + 15, 19);
        scanf("%c", &cSel);

        if (cSel == 'Q') {
            printXY(WIDTH / 4, 21, "               ");
            setColor(BLACK, MAGENTA2);
            printXY(WIDTH / 4, 21, " > Quit ");
            setColor(WHITE, BLACK);
            printXY(47, 21, "              ");
            gotoXY(0, HEIGHT - 1);
            return iSel = 0;
        } else if (cSel == 'F') {
            printXY(WIDTH / 4, 21, "               ");
            printXY(WIDTH / 4, 21, "Start game         ");
            Sleep(1000);
            return iSel = 1;
        } else if (cSel == 'S') {
            printXY(WIDTH / 4, 21, "        ");
            setColor(BLACK, YELLOW2);
            printXY(WIDTH / 4, 21, " > Show score ");
            setColor(WHITE, BLACK);
            printXY(52, 21, "      ");
            Sleep(1000);
            return iSel = 2;
        }
        //else {
        //    printXY(38, 21, "                   ");
        //    setColor(WHITE, RED2);
        //    printXY(38, 21, " > Please enter again.");
        //    setColor(WHITE, BLACK);
        //    gotoXY(50, 19);
        //}

    }
}

// read, draws score
void score_menu(FILE *pF) {
    int i;
    char cSel, output[50];
    Score temp[10] = { 0, };

    cls(WHITE, BLACK);

    drawBox(0, 0, WIDTH - 2, HEIGHT - 2);

    fseek(pF, 0, SEEK_SET);
    for (i = 0; i < 10; i++) {
        fread(&temp[i], sizeof(Score), 1, pF);
    }


    while (1) {
        printXY(WIDTH / 4, 8, "┌────<Score  Board>────┐");
        printXY(WIDTH / 4, 9, "│                              │");
        for (i = 0; i < 10; i++) {
            sprintf(output, "│        [%02d] %3s %4d         │\n", i + 1, temp[i].name, temp[i].score);
            printXY(WIDTH / 4, 10 + i, output);
        }
        printXY(WIDTH / 4, 20, "│                              │");
        printXY(WIDTH / 4, 21, "└───────────────┘");

        printXY(WIDTH / 4 + 3, 22, "> Return to main [R] :  ");
        showCursor();
        gotoXY(WIDTH / 4 + 26, 22);
        scanf("%c", &cSel);

        if (cSel == 'R') {
            printXY(WIDTH / 4 + 3, 24, "                   ");
            setColor(BLACK, YELLOW2);
            printXY(WIDTH / 4 + 3, 24, "> Returning...");
            setColor(WHITE, BLACK);
            Sleep(1000);
            return;
        }
    }

}