// 2022 Programming Lab

#define _CRT_SECURE_NO_WARNINGS

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"

#define INIT_BULLET 3
#define INIT_TRACKER_BULLET 1
#define INIT_ZIG_BULLET 2

#define TOTAL_BULLET 20
#define TOTAL_ZIGZAG_BULLET 5
#define TOTAL_TRACKER_BULLET 5
#define TOTAL_GOLD 5

#define ZIGZAG_SPEED 8

/* 일반 별 */
Bullet bullet_arr[TOTAL_BULLET];

/* 지그재그 별*/
Bullet bullet_zig_arr[TOTAL_ZIGZAG_BULLET];

/* 추격하는 별 */
Tracker_Bullet bullet_tracker_arr[TOTAL_TRACKER_BULLET];

Player p1;
Gold gold_arr[TOTAL_GOLD];
Heart heart_arr[1];

//int bullet_num;         // old *bullet_count
//int bullet_tracker_num;

/*
* 방향(orientation)
*         0
*      7  ↑  1
*    6  ←   →  2
*      5  ↓  3
*         4
*/



/* CREATE */
void bullet_create(Gamedata* gamedata) {
	int init_point, rand_color, rand_speed, rand_shape, pX;

	const int color[3] = { 4, 14, 6 };
	const int speed[3] = { 4, 8, 16 };

	rand_color = rand() % 3;            // 총알 컬러 랜덤 지정
	bullet_arr[gamedata->bullet_count].bColor = color[rand_color];

	rand_speed = rand() % 100;
	if (rand_speed < 10) {
		bullet_arr[gamedata->bullet_count].bSpeed = speed[0];
	} else if (rand_speed < 50 && rand_speed >= 10) {
		bullet_arr[gamedata->bullet_count].bSpeed = speed[1];
	} else {
		bullet_arr[gamedata->bullet_count].bSpeed = speed[2];
	}

	rand_shape = rand() % 2;
	bullet_arr[gamedata->bullet_count].bShape = rand_shape;

	init_point = rand() % 4;                // 화면의 4분면 중 어디서 스폰할지

	switch (init_point) {
		// 제 1,2 사분면 (위)
		case 0:
			pX = rand() % (WIDTH - 5) + 2;
			if (pX % 2) {
				--pX;
			}
			bullet_arr[gamedata->bullet_count].bX = pX;
			bullet_arr[gamedata->bullet_count].bY = 2;
			bullet_arr[gamedata->bullet_count].bOrient = rand() % 3 + 3;
			break;
			// 제 2, 3 사분면 (좌)
		case 1:
			bullet_arr[gamedata->bullet_count].bX = 2;
			bullet_arr[gamedata->bullet_count].bY = rand() % (HEIGHT - 10) + 2;
			bullet_arr[gamedata->bullet_count].bOrient = rand() % 3 + 1;
			break;
			// 제 3, 4 사분면 (아래)
		case 2:
			pX = rand() % (WIDTH - 6) + 2;
			if (pX % 2) {
				--pX;
			}
			bullet_arr[gamedata->bullet_count].bX = pX;
			bullet_arr[gamedata->bullet_count].bY = HEIGHT - 4;
			bullet_arr[gamedata->bullet_count].bOrient = (rand() % 3 + 7) % 8; // { ([0, 1, 2] + 7) = [7, 8, 9] } % 8 = [7, 0, 1]
			break;
			// 제 4, 1 사분면 (우)
		case 3:
			bullet_arr[gamedata->bullet_count].bX = WIDTH - 4;
			bullet_arr[gamedata->bullet_count].bY = rand() % (HEIGHT - 10) + 2;
			bullet_arr[gamedata->bullet_count].bOrient = rand() % 3 + 5;
			break;
	}
	++(gamedata->bullet_count);
}

void bullet_zig_create(Gamedata* gamedata) {
	int init_point, pX;

	bullet_zig_arr[gamedata->bullet_zig_count].bColor = 11;
	bullet_zig_arr[gamedata->bullet_zig_count].bSpeed = ZIGZAG_SPEED;
	bullet_zig_arr[gamedata->bullet_zig_count].bShape = 0;

	init_point = rand() % 4;                // 화면의 4분면 중 어디서 스폰할지

	switch (init_point) {
		// 제 1,2 사분면 (위)
		case 0:
			pX = rand() % (WIDTH - 5) + 2;
			if (pX % 2) {
				--pX;
			}
			bullet_zig_arr[gamedata->bullet_zig_count].bX = pX;
			bullet_zig_arr[gamedata->bullet_zig_count].bY = 2;
			bullet_zig_arr[gamedata->bullet_zig_count].bOrient = 3;
			break;
			// 제 2, 3 사분면 (좌)
		case 1:
			bullet_zig_arr[gamedata->bullet_zig_count].bX = 2;
			bullet_zig_arr[gamedata->bullet_zig_count].bY = rand() % (HEIGHT - 10) + 2;
			bullet_zig_arr[gamedata->bullet_zig_count].bOrient = 1;
			break;
			// 제 3, 4 사분면 (아래)
		case 2:
			pX = rand() % (WIDTH - 6) + 2;
			if (pX % 2) {
				--pX;
			}
			bullet_zig_arr[gamedata->bullet_zig_count].bX = pX;
			bullet_zig_arr[gamedata->bullet_zig_count].bY = HEIGHT - 4;
			bullet_zig_arr[gamedata->bullet_zig_count].bOrient = 7;
			break;
			// 제 4, 1 사분면 (우)
		case 3:
			bullet_zig_arr[gamedata->bullet_zig_count].bX = WIDTH - 4;
			bullet_zig_arr[gamedata->bullet_zig_count].bY = rand() % (HEIGHT - 10) + 2;
			bullet_zig_arr[gamedata->bullet_zig_count].bOrient = 5;
			break;
	}
	++(gamedata->bullet_zig_count);
}

/* 추격하는 총알*/
void bullet_tracker_create(Gamedata* gamedata) {
	int rand_pos = rand() % 4;
	switch (rand_pos) {
		case 0:
			bullet_tracker_arr[gamedata->bullet_tracker_count].bX = 4;
			bullet_tracker_arr[gamedata->bullet_tracker_count].bY = 4;
			break;
		case 1:
			bullet_tracker_arr[gamedata->bullet_tracker_count].bX = WIDTH - 4;
			bullet_tracker_arr[gamedata->bullet_tracker_count].bY = 4;
			break;
		case 2:
			bullet_tracker_arr[gamedata->bullet_tracker_count].bX = WIDTH - 4;
			bullet_tracker_arr[gamedata->bullet_tracker_count].bY = HEIGHT - 4;
			break;
		case 3:
			bullet_tracker_arr[gamedata->bullet_tracker_count].bX = 4;
			bullet_tracker_arr[gamedata->bullet_tracker_count].bY = HEIGHT - 4;
			break;
	}

	bullet_tracker_arr[gamedata->bullet_tracker_count].bColor = RED2;

	++(gamedata->bullet_tracker_count);
}

/* CLEAR */
void bullet_clear(Gamedata* gamedata) {
	for (int i = 0; i < gamedata->bullet_count; i++) {
		bufferPrintXY(bullet_arr[i].bX, bullet_arr[i].bY, " ");
	}
}

void bullet_zig_clear(Gamedata* gamedata) {
	for (int i = 0; i < gamedata->bullet_zig_count; i++) {
		bufferPrintXY(bullet_zig_arr[i].bX, bullet_zig_arr[i].bY, " ");
	}
}

void bullet_tracker_clear(Gamedata* gamedata) {
	for (int i = 0; i < gamedata->bullet_tracker_count; i++) {
		bufferPrintXY(bullet_tracker_arr[i].bX, bullet_tracker_arr[i].bY, " ");
	}
}

/* MOVE */
void bullet_move(int *px, int *py, int orient) {
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

void bullet_zig_move(int* px, int* py, int orient, int frame_count) {
	int switch_zig;
	switch_zig = ZIGZAG_SPEED * 10;

	switch (orient) {
		/* 왼쪽스폰 = 대각선위-대각선아래-... */
		case 1:
			if (frame_count % switch_zig < (int) switch_zig * 0.5) {
				*px += 2;
				*py -= 1;
			} else {
				*px += 2;
				*py += 1;
			}
			break;
		/* 위 스폰 = 오른쪽아래 - 왼쪽아래 - ... */
		case 3:
			if (frame_count % switch_zig < (int) switch_zig * 0.5) {
				*px += 2;
				*py += 1;
			} else {
				*px -= 2;
				*py += 1;
			}
			break;
		/* 오른쪽 스폰 = 대각선 아래 - 대각선위-... */
		case 5:
			if (frame_count % switch_zig < (int) switch_zig * 0.5) {
				*px -= 2;
				*py += 1;
			} else {
				*px -= 2;
				*py -= 1;
			}
			break;
		/* 아래 스폰 = 왼쪽위 - 오른쪽위 - ... */
		case 7:
			if (frame_count % switch_zig < (int) switch_zig * 0.5) {
				*px -= 2;
				*py -= 1;
			} else {
				*px += 2;
				*py -= 1;
			}
			break;
	}
}

void bullet_tracker_move(int *bx, int *by) {
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

/* DELETE */
/*
 bullet_arr[]의 n번째 총알 삭제(화면에서 지우는게 아님)
 = 뒤에서 앞으로 한칸씩 땡겨서 n번째 인덱스의 총알 제거
*/
void bullet_delete(int n, Gamedata* gamedata) {
	int i;
	for (i = n; i < gamedata->bullet_count - 1; i++) {
		bullet_arr[i].bX = bullet_arr[i + 1].bX;
		bullet_arr[i].bY = bullet_arr[i + 1].bY;
		bullet_arr[i].bColor = bullet_arr[i + 1].bColor;
		bullet_arr[i].bOrient = bullet_arr[i + 1].bOrient;
		bullet_arr[i].bSpeed = bullet_arr[i + 1].bSpeed;
		bullet_arr[i].bShape = bullet_arr[i + 1].bShape;
	}
	--(gamedata->bullet_count);
}

void bullet_zig_delete(int n, Gamedata* gamedata) {
	int i;
	for (i = n; i < gamedata->bullet_zig_count - 1; i++) {
		bullet_zig_arr[i].bX = bullet_zig_arr[i + 1].bX;
		bullet_zig_arr[i].bY = bullet_zig_arr[i + 1].bY;
		bullet_zig_arr[i].bColor = bullet_zig_arr[i + 1].bColor;
		bullet_zig_arr[i].bOrient = bullet_zig_arr[i + 1].bOrient;
		bullet_zig_arr[i].bSpeed = bullet_zig_arr[i + 1].bSpeed;
		bullet_zig_arr[i].bShape = bullet_zig_arr[i + 1].bShape;
	}
	--(gamedata->bullet_zig_count);
}

void bullet_tracker_delete(int n, Gamedata* gamedata) {
	for (int i = n; i < gamedata->bullet_tracker_count - 1; i++) {
		bullet_tracker_arr[i].bX = bullet_tracker_arr[i + 1].bX;
		bullet_tracker_arr[i].bY = bullet_tracker_arr[i + 1].bY;
		bullet_tracker_arr[i].bColor = bullet_tracker_arr[i + 1].bColor;
	}
	--(gamedata->bullet_tracker_count);
}

/* PRINT */
void bullet_print(Gamedata* gamedata) {
	int newBullet;
	newBullet = 0;

	for (int i = 0; i < gamedata->bullet_count; i++) {
		if ((gamedata->frame_count % bullet_arr[i].bSpeed) == 0) {
			bullet_move(&bullet_arr[i].bX, &bullet_arr[i].bY, bullet_arr[i].bOrient);
		}

		if (bullet_arr[i].bX < 2 || bullet_arr[i].bX > WIDTH - 3) {
			bullet_delete(i, gamedata);
			--i;
			++newBullet;
		} else if (bullet_arr[i].bY < 1 || bullet_arr[i].bY > HEIGHT - 3) {
			bullet_delete(i, gamedata);
			--i;
			++newBullet;
		}
	}

	/* 제거된 만큼 다시 생성 */
	for (int i = 0; i < newBullet; i++) {
		bullet_create(gamedata);
	}

	/* 실제로 탄막을 프린트하는 부분 */
	for (int i = 0; i < gamedata->bullet_count; i++) {
		bufferSetColor(bullet_arr[i].bColor, BLACK);
		if (bullet_arr[i].bShape) {
			bufferPrintXY(bullet_arr[i].bX, bullet_arr[i].bY, "★");
		} else {
			bufferPrintXY(bullet_arr[i].bX, bullet_arr[i].bY, "☆");
		}
		bufferSetColor(WHITE, BLACK);
	}
}

void bullet_zig_print(Gamedata* gamedata) {
	int newBullet;
	newBullet = 0;

	for (int i = 0; i < gamedata->bullet_zig_count; i++) {
		if ((gamedata->frame_count % bullet_zig_arr[i].bSpeed) == 0) {
			bullet_zig_move(&bullet_zig_arr[i].bX, &bullet_zig_arr[i].bY, bullet_zig_arr[i].bOrient, gamedata->frame_count);
		}

		if (bullet_zig_arr[i].bX < 2 || bullet_zig_arr[i].bX > WIDTH - 3) {
			bullet_zig_delete(i, gamedata);
			--i;
			++newBullet;
		} else if (bullet_zig_arr[i].bY < 1 || bullet_zig_arr[i].bY > HEIGHT - 3) {
			bullet_zig_delete(i, gamedata);
			--i;
			++newBullet;
		}
	}

	/* 제거된 만큼 다시 생성 */
	for (int i = 0; i < newBullet; i++) {
		bullet_zig_create(gamedata);
	}

	/* 실제로 탄막을 프린트하는 부분 */
	for (int i = 0; i < gamedata->bullet_zig_count; i++) {
		bufferSetColor(bullet_zig_arr[i].bColor, BLACK);

		bufferPrintXY(bullet_zig_arr[i].bX, bullet_zig_arr[i].bY, "★");

		bufferSetColor(WHITE, BLACK);
	}
}

void bullet_tracker_print(Gamedata* gamedata) {
	int newBullet;
	newBullet = 0;

	for (int i = 0; i < gamedata->bullet_tracker_count; i++) {
		// 총알을 한 프레임 
		if (gamedata->frame_count % 15 == 0) {
			bullet_tracker_move(&bullet_tracker_arr[i].bX, &bullet_tracker_arr[i].bY);
		}

		/* 화면 밖으로 나가면 탄막 배열에서 제거 */
		if (bullet_tracker_arr[i].bX < 3 || bullet_tracker_arr[i].bX > WIDTH - 3) {
			bullet_tracker_delete(i, gamedata);
			i--;
			newBullet++;
		} else if (bullet_tracker_arr[i].bY < 1 || bullet_tracker_arr[i].bY > HEIGHT - 3) {
			bullet_tracker_delete(i, gamedata);
			i--;
			newBullet++;
		}
	}

	/* 제거된 만큼 다시 생성 */
	for (int i = 0; i < newBullet; i++) {
		bullet_tracker_create(gamedata);
	}

	/* 실제로 탄막을 프린트하는 부분 */
	for (int i = 0; i < gamedata->bullet_tracker_count; i++) {
		bufferSetColor(bullet_tracker_arr[i].bColor, BLACK);
		bufferPrintXY(bullet_tracker_arr[i].bX, bullet_tracker_arr[i].bY, "★");
		bufferSetColor(WHITE, BLACK);
	}
}

/* GOLD */
void gold_create(Gamedata* gamedata) {
	int p_x, p_y;
	p_x = rand() % (WIDTH - 4) + 2;
	if (p_x % 2) {
		--p_x;
	}
	p_y = rand() % (HEIGHT - 4) + 1;
	gold_arr[gamedata->gold_count].x = p_x;
	gold_arr[gamedata->gold_count].y = p_y;

	++(gamedata->gold_count);

}

void gold_delete(int n, Gamedata* gamedata) {
	for (int i = n; i < gamedata->gold_count - 1; i++) {
		gold_arr[i].x = gold_arr[i + 1].x;
		gold_arr[i].y = gold_arr[i + 1].y;
	}
	--(gamedata->gold_count);
}

void gold_clear(Gamedata* gamedata) {
	for (int i = 0; i < gamedata->gold_count; i++) {
		bufferPrintXY(gold_arr[i].x, gold_arr[i].y, " ");
	}
}

void gold_print(Gamedata* gamedata) {
	for (int i = 0; i < gamedata->gold_count; i++) {
		bufferSetColor(GREEN2, BLACK);
		bufferPrintXY(gold_arr[i].x, gold_arr[i].y, "＄");
		bufferSetColor(WHITE, BLACK);
	}
}

/* HEART */
void heart_create(Gamedata* gamedata) {
	int p_x, p_y;
	p_x = rand() % (WIDTH - 4) + 2;
	if (p_x % 2) {
		--p_x;
	}
	p_y = rand() % (HEIGHT - 4) + 1;
	heart_arr[0].x = p_x;
	heart_arr[0].y = p_y;
	heart_arr[0].heart_true = 1;
}

void heart_delete(void) {
	heart_arr[0].x = 0;
	heart_arr[0].y = 0;
	heart_arr[0].heart_true = 0;
}

void heart_clear(void) {
	bufferPrintXY(heart_arr[0].x, heart_arr[0].y, " ");
}

void heart_print(Heart* heart) {
	if (heart->heart_true) {
		bufferSetColor(RED2, BLACK);
		bufferPrintXY(heart_arr[0].x, heart_arr[0].y, "♥");
		bufferSetColor(WHITE, BLACK);
	}
}

/* 플레이어 이동 : https://coding-factory.tistory.com/693 참고 */
void player1(Gamedata* gamedata) {
	bufferPrintXY(p1.pX, p1.pY, "　");

	if (gamedata->frame_count % 3 == 0) {
		if (GetAsyncKeyState(VK_LEFT) & 0x8000) { //왼쪽
			p1.pX -= 2;
			if (p1.pX <= 1) {
				p1.pX = 2;
			}
		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000) { //오른쪽
			p1.pX += 2;
			if (p1.pX >= WIDTH - 3) {
				p1.pX = WIDTH - 4;
			}
		}
		if (GetAsyncKeyState(VK_UP) & 0x8000) { //위
			p1.pY--;
			if (p1.pY <= 0) {
				p1.pY = 1;
			}
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000) { //아래
			p1.pY++;
			if (p1.pY >= HEIGHT - 2) {
				p1.pY = HEIGHT - 3;
			}
		}
	}


	bufferSetColor(GREEN1, BLACK);
	bufferPrintXY(p1.pX, p1.pY, "Ω");
	bufferSetColor(WHITE, BLACK);
}

void print_score(time_t start, Gamedata* gamedata) {
	int delta;
	time_t now;
	char c_time[WIDTH];
	char temp[WIDTH];

	now = time(NULL);
	delta = (int) (now - start);
	gamedata->current_score = delta + gamedata->gold_score;

	bufferSetColor(BLACK, GRAY1);

	for (int i = 0; i < WIDTH; i++) {
		bufferPrintXY(i, HEIGHT - 2, " ");
	}


	sprintf(c_time, "SCORE:%3d",
		gamedata->current_score);
	bufferPrintXY(2, HEIGHT - 2, c_time);

	if (p1.life >= 3) {
		sprintf(temp, "♥♥♥");
	} else if (p1.life == 2) {
		sprintf(temp, "♥♥♡");
	} else if (p1.life == 1) {
		sprintf(temp, "♥♡♡");
	} else {
		sprintf(temp, "♨");
	}

	sprintf(c_time, "LIFE[%2d]", p1.life);
	bufferPrintXY(WIDTH - 17, HEIGHT - 2, c_time);
	bufferSetColor(RED2, GRAY1);
	bufferPrintXY(WIDTH - 8, HEIGHT - 2, temp);
	bufferSetColor(WHITE, BLACK);
}

void debug_print_score(time_t start, Gamedata* gamedata) {
	int delta;
	time_t now;
	char c_time[WIDTH];

	now = time(NULL);
	delta = (int) (now - start);
	gamedata->current_score = delta + gamedata->gold_score;

	bufferSetColor(BLACK, GRAY1);

	for (int i = 0; i < WIDTH; i++) {
		bufferPrintXY(i, HEIGHT - 2, " ");
	}
	sprintf(c_time, "score:%3d  |  f:%3d  |  b_n:%2d  |  life:%d",
		gamedata->current_score, gamedata->frame_count,
		gamedata->bullet_count, p1.life);
	bufferPrintXY(2, HEIGHT - 2, c_time);

	bufferSetColor(WHITE, BLACK);
}

void check_collision(Gamedata* gamedata) {
	/* collision check */

	/* normal star */
	for (int i = 0; i < gamedata->bullet_count; i++) {
		if ((p1.pX == bullet_arr[i].bX) && (p1.pY == bullet_arr[i].bY)) {
			if (p1.life > 0) {
				--p1.life;
				bullet_delete(i, gamedata);
			}
			break;
		}
	}		
	/* zigzag star */
	for (int i = 0; i < gamedata->bullet_zig_count; i++) {
		if ((p1.pX == bullet_zig_arr[i].bX) && (p1.pY == bullet_zig_arr[i].bY)) {
			if (p1.life > 0) {
				--p1.life;
				bullet_zig_delete(i, gamedata);
			}
			break;
		}
	}	
	/* tracker star */
	for (int i = 0; i < gamedata->bullet_tracker_count; i++) {
		if ((p1.pX == bullet_tracker_arr[i].bX) && (p1.pY == bullet_tracker_arr[i].bY)) {
			if (p1.life > 0) {
				--p1.life;
				bullet_tracker_delete(i, gamedata);
			}
			break;
		}
	}
	/* gold */
	for (int i = 0; i < gamedata->gold_count; i++) {
		if ((p1.pX == gold_arr[i].x) && (p1.pY == gold_arr[i].y)) {
			gold_delete(i, gamedata);
			gamedata->gold_score += 100;
			break;
		}
	}	
	/* heart */
	if ((p1.pX == heart_arr[0].x) && (p1.pY == heart_arr[0].y)) {
		heart_delete();
		p1.life += 1;
	}
	/* collision check end */
}

void gamedata_init(Gamedata* gamedata) {
	gamedata->bullet_count = 0;
	gamedata->bullet_zig_count = 0;
	gamedata->gold_count = 0;
	gamedata->bullet_tracker_count = 0;
	gamedata->current_score = 0;
	gamedata->gold_score = 0;
	gamedata->frame_count = 0;
}

void init_bg(int bg[][WIDTH - 4]) {
	int p_x, p_y;

	for (int i = 0; i < 10; i++) {
		do {
			p_y = rand() % (HEIGHT - 6) + 2;
			p_x = rand() % (WIDTH - 6);
		} while (bg[p_y][p_x] != 0);

		bg[p_y][p_x] = 1;
	}
}

void draw_bg(int bg[][WIDTH - 4], int frame_count) {
	for (int i = 0; i < HEIGHT - 2; i++) {
		for (int j = 0; j < WIDTH - 4; j++) {
			if (bg[i][j]) {
				if (frame_count % 20 < 10) {
					bufferSetColor(WHITE, BLACK);
					bufferPrintXY(j, i, "＊");
				} else {
					bufferSetColor(GRAY1, BLACK);
					bufferPrintXY(j, i, "†");
				}
			}
		}
	}
}

void game_over(Score* result, Gamedata* gamedata) {
	int game_over_logo[10][57] = {
	{0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
	{0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
	{0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
	{0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1},
	{0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1},
	{0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1},
	{0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1},
	{0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0},
	{0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1},
	{0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1}
	};

	int pos_x;
	pos_x = 0;

	cls(WHITE, BLACK);
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 57; j++) {
			pos_x = j * 2;
			if (game_over_logo[i][j]) {
				if (i < 3) {
					setColor(CYAN2, CYAN2);
				} else if (i < 6) {
					setColor(CYAN1, CYAN1);
				} else if (i < 9) {
					setColor(BLUE2, BLUE2);
				} else {
					setColor(BLUE1, BLUE1);
				}
				printXY((WIDTH - 57) / 2 + pos_x, i + 1, "■");
			} else {
				setColor(BLACK, BLACK);
				printXY((WIDTH - 57) / 2 + pos_x, i + 1, "■");
			}
		}
	}

	setColor(WHITE, BLACK);
	printXY((WIDTH - 11) / 2, HEIGHT / 2, "YOU DIED!!");
	printXY((WIDTH - 34) / 2, HEIGHT / 2 + 1, "> ENTER NICKNAME (3 CHARACTERS): ");
	Sleep(1000);

	do {
		setColor(WHITE, BLACK);
		printXY((WIDTH - 13) / 2, HEIGHT / 2 + 2, "            ");
		gotoXY((WIDTH - 13) / 2 + 4, HEIGHT / 2 + 2);
		showCursor();
		scanf(" %s", &result->name);
	} while (strlen(result->name) != 3);


	result->score = gamedata->current_score;
}

void game(Score* result) {
	int backgroud[HEIGHT - 2][WIDTH - 4] = { 0, };

	Gamedata gamedata;
	time_t start;

	p1.pX = WIDTH / 2;
	p1.pY = HEIGHT / 2;
	p1.life = 3;

	gamedata_init(&gamedata);
	init_bg(backgroud);

	/*
	* TOTAL_BULLET 개수 만큼 배열에 총알 정보 저장(생성?)
	* ex) 스폰위치, 방향, 속도, 색깔, 모양
	* -> 이후 bullet_count = 루프 돌린 개수(INIT_BULLET)만큼 됨
	*/
	for (int i = 0; i < INIT_BULLET; i++) {
		bullet_create(&gamedata);
	}

	for (int i = 0; i < INIT_ZIG_BULLET; i++) {
		bullet_zig_create(&gamedata);
	}
	start = time(NULL);

	/* double buffer start >> */
	scr_init();
	while (1) {
		scr_switch();
		scr_clear();

		print_score(start, &gamedata);
		//debug_print_score(start, &gamedata);
		draw_bg(backgroud, gamedata.frame_count);

		player1(&gamedata);

		bullet_clear(&gamedata);
		bullet_print(&gamedata);

		bullet_zig_clear(&gamedata);
		bullet_zig_print(&gamedata);

		gold_clear(&gamedata);
		gold_print(&gamedata);

		bullet_tracker_clear(&gamedata);
		bullet_tracker_print(&gamedata);

		heart_clear();
		heart_print(&heart_arr[0]);

		if ((gamedata.frame_count % 400 == 0) && (gamedata.gold_count < TOTAL_GOLD)) {
			gold_create(&gamedata);
		}

		if (gamedata.current_score > 500) {
			if ((gamedata.frame_count % 200 == 0) && (gamedata.bullet_count < TOTAL_BULLET)) {
				bullet_create(&gamedata);
			}			
			if ((gamedata.frame_count % 200 == 0) && (gamedata.bullet_zig_count < TOTAL_ZIGZAG_BULLET)) {
				bullet_zig_create(&gamedata);
			}
			if ((gamedata.frame_count % 800 == 0) && (gamedata.bullet_tracker_count < TOTAL_TRACKER_BULLET)) {
				bullet_tracker_create(&gamedata);
			}
			if ((gamedata.frame_count % 800 == 0) && (p1.life < 2)) {
				heart_create(&gamedata);
			}
		}

		check_collision(&gamedata);

		if (p1.life == 0) {
			break;
		} else {
			++(gamedata.frame_count);
		}
		/* end loop */
	}
	scr_clear();
	scr_release();
	/* >> double buffer end */

	game_over(result, &gamedata);
}

int main() {

	/* Main menu */
	while (1) {
		int select;
		FILE *pF;
		Score temp = { 0, };
		Score temp_arr[10] = { 0, };

		pF = fopen(DATA, "r+");
		if (pF == NULL) {
			pF = fopen(DATA, "w+");
			if (pF == NULL) {
				printXY(0, HEIGHT - 1, "Can not open file. Exiting...");
				exit(1);
			}
		}

		srand(( unsigned int )time(NULL));

		init();
		draw_main_menu();

		draw_main_menu_sel(&select);

		switch (select) {
			/* Game quit */
			case 0:
				fclose(pF);
				exit(0);
			/* main game */
			case 1:
				cls(WHITE, BLACK);
				timer_before_game();

				game(&temp);

				cls(WHITE, BLACK);
				printXY(WIDTH / 4, HEIGHT / 2, temp.name);

				fseek(pF, 0, SEEK_SET);
				for (int i = 0; i < 10; i++) {
					fread(&temp_arr[i], sizeof(Score), 1, pF);
				}
				qsort(temp_arr, 10, sizeof(Score), compare);

				strcpy(temp_arr[9].name, temp.name);
				temp_arr[9].score = temp.score;

				fseek(pF, 0, SEEK_SET);
				for (int i = 0; i < 10; i++) {
					fwrite(&temp_arr[i], sizeof(Score), 1, pF);
				}

				fclose(pF);
				break;
			/* Score board */
			case 2:
				score_menu(pF);
				fclose(pF);
				break;
		}
	}
	return 0;
}