// 2022 Programming Lab

#define _CRT_SECURE_NO_WARNINGS

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"

#define TOTAL_BULLET 20
#define TOTAL_TRACKER_BULLET 5

typedef struct _Bullet {
	int bX;
	int bY;
	int bColor;         // 1 ~ 15 �÷��ڵ�
	int bOrient;        // 0 : 12��, 1 : 1~2��, 2 : ����, ... , 6 : 9��, 7 : 10 ~ 11�� ����
	int bSpeed;
	int bShape;
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
*         0
*      7  ��  1
*    6  ��   ��  2
*      5  ��  3
*         4
*/

// ź��(�Ѿ�) ����
void bullet_create(void) {
	int init_point, rand_color, rand_speed, rand_shape, pX;
	int color[3] = { 6, 14, 15 };
	int speed[3] = { 4, 12, 20 };

	rand_color = rand() % 3;            // �Ѿ� �÷� ���� ����
	bullet_arr[bullet_num].bColor = color[rand_color];

	rand_speed = rand() % 100;
	if (rand_speed < 10) {
		bullet_arr[bullet_num].bSpeed = speed[0];
	} else if (rand_speed < 50 && rand_speed >= 10) {
		bullet_arr[bullet_num].bSpeed = speed[1];
	} else {
		bullet_arr[bullet_num].bSpeed = speed[2];
	}

	rand_shape = rand() % 2;
	bullet_arr[bullet_num].bShape = rand_shape;

	init_point = rand() % 4;                // ȭ���� 4�и� �� ��� ��������

	switch (init_point) {
		// �� 1,2 ��и� (��)
		case 0:
			pX = rand() % (WIDTH - 5) + 2;
			if (pX % 2) {
				++pX;
			}
			bullet_arr[bullet_num].bX = pX;
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
			pX = rand() % (WIDTH - 6) + 2;
			if (pX % 2) {
				++pX;
			}
			bullet_arr[bullet_num].bX = pX;
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

	bullet_tracker_arr[bullet_tracker_num].bColor = CYAN1;

	++bullet_tracker_num;
}

void bullet_clear(void) {
	for (int i = 0; i < bullet_num; i++) {
		bufferPrintXY(bullet_arr[i].bX, bullet_arr[i].bY, " ");
	}
}

void bullet_tracker_clear(int diff) {
	for (int i = 0; i < bullet_tracker_num - 1 - diff; i++) {
		bufferPrintXY(bullet_tracker_arr[i].bX, bullet_tracker_arr[i].bY, " ");
	}
}

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
		bullet_arr[i].bSpeed = bullet_arr[i + 1].bSpeed;
		bullet_arr[i].bShape = bullet_arr[i + 1].bShape;
	}
	bullet_num--;
}

void bullet_tracker_delete(int n) {
	for (int i = n; i < bullet_tracker_num - 1; i++) {
		bullet_tracker_arr[i].bX = bullet_tracker_arr[i + 1].bX;
		bullet_tracker_arr[i].bY = bullet_tracker_arr[i + 1].bY;
		bullet_tracker_arr[i].bColor = bullet_tracker_arr[i + 1].bColor;
	}
	bullet_tracker_num--;
}

void bullet_print(int frame_count, int diff) {
	int newBullet;
	newBullet = 0;

	for (int i = 0; i < bullet_num - diff; i++) {
		if (frame_count % bullet_arr[i].bSpeed == 0) {
			bullet_move(&bullet_arr[i].bX, &bullet_arr[i].bY, bullet_arr[i].bOrient);
		}

		/* ȭ�� ������ ������ ź�� �迭���� ���� */
		if (bullet_arr[i].bX < 2 || bullet_arr[i].bX > WIDTH - 3) {
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
	for (int i = 0; i < bullet_num - diff; i++) {
		bufferSetColor(bullet_arr[i].bColor, BLACK);
		if (bullet_arr[i].bShape) {
			bufferPrintXY(bullet_arr[i].bX, bullet_arr[i].bY, "��");
		} else {
			bufferPrintXY(bullet_arr[i].bX, bullet_arr[i].bY, "��");
		}
		bufferSetColor(WHITE, BLACK);
	}
}

void bullet_tracker_print(int frame_count, int diff) {
	int newBullet;
	newBullet = 0;

	for (int i = 0; i < bullet_tracker_num - diff; i++) {
		// �Ѿ��� �� ������ 
		if (frame_count % 10 == 0) {
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
void player1(int frame_count) {
	bufferPrintXY(p1.pX, p1.pY, " ");

	if (frame_count % 3 == 0) {
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

void print_score(clock_t start, int* score, int frame, int diff) {
	char time[WIDTH];
	char temp[WIDTH];

	clock_t now, delta;

	now = clock();
	delta = (now - start) / 100;

	bufferSetColor(BLACK, GRAY1);

	for (int i = 0; i < WIDTH; i++) {
		bufferPrintXY(i, HEIGHT - 2, " ");
	}

	//sprintf(time, "Score: %d", delta);
	sprintf(time, "Score: %d-%d", frame, diff);
	bufferPrintXY(WIDTH / 2 - 16, HEIGHT - 2, time);

	//if (p1.life == 3) {
	//	sprintf(temp, "Life: ������");
	//} else if (p1.life == 2) {
	//	sprintf(temp, "Life: ������");
	//} else if (p1.life == 1) {
	//	sprintf(temp, "Life: ������");
	//} else {
	//	sprintf(temp, "��");
	//}

	switch (p1.life) {
		case 1:
			sprintf(temp, "Life: ������");
			break;
		case 2:
			sprintf(temp, "Life: ������");
			break;
		case 3:
			sprintf(temp, "Life: ������");
			break;
		default:
			sprintf(temp, "��");
			break;
	}

	bufferPrintXY(WIDTH / 2, HEIGHT - 2, temp);

	bufferSetColor(WHITE, BLACK);

	*score = (int) delta;

	//return ( unsigned long )delta;
}

void check_collision(void) {
	/* collision check */
	for (int i = 0; i < TOTAL_BULLET; i++) {
		if ((p1.pX == bullet_arr[i].bX) && (p1.pY == bullet_arr[i].bY)) {
			if (p1.life > 0) {
				--p1.life;
				bullet_delete(i);
			}
			break;
		}
	}
	for (int i = 0; i < TOTAL_TRACKER_BULLET; i++) {
		if ((p1.pX == bullet_tracker_arr[i].bX) && (p1.pY == bullet_tracker_arr[i].bY)) {
			if (p1.life > 0) {
				--p1.life;
				bullet_tracker_delete(i);
			}
			break;
		}
	}
	/* collision check end */
}

void game(Score *result) {
	int frame_count, tracker_increase, diff;
	clock_t start;
	int current_score;

	diff = TOTAL_BULLET * 7 / 8;
	tracker_increase = TOTAL_TRACKER_BULLET;

	p1.pX = WIDTH / 2;
	p1.pY = HEIGHT / 2;
	//p1.life = 3;
	p1.life = 300;

	bullet_num = 0;
	bullet_tracker_num = 0;

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

		print_score(start, &current_score, frame_count, diff);

		player1(frame_count);

		bullet_clear();
		bullet_print(frame_count, diff);

		if ((current_score  = 501) > 500) {
			if ((diff > 0) && (frame_count % 250 == 0)) {
				--diff;
				frame_count = 0;
			}
			//bullet_tracker_clear(tracker_increase);
			//bullet_tracker_print(frame_count, tracker_increase);
		}

		check_collision();

		/* [life = 0] -> game end */
		if (p1.life == 0) {
			break;
		} else {
			++frame_count;
		}
		/* end loop */
	}
	scr_clear();
	scr_release();
	/* >> double buffer end */

	cls(WHITE, BLACK);
	printXY(WIDTH / 4 + 12, HEIGHT / 2, "YOU DIED!!");
	printXY(WIDTH / 4, HEIGHT / 2 + 1, "> ENTER NICKNAME (3 CHARACTERS): ");
	Sleep(1000);

	do {
		setColor(WHITE, BLACK);
		printXY(WIDTH / 4 + 14, HEIGHT / 2 + 2, "            ");
		gotoXY(WIDTH / 4 + 14, HEIGHT / 2 + 2);
		showCursor();
		scanf(" %s", &result->name);
	} while (strlen(result->name) != 3);


	result->score = ( int )current_score;
}

int main() {
	FILE *pF;
	//Score scr[3] = { {"AAA", 1111}, {"BBB", 999}, {"CCC", 3333} };

	/* Main menu */
	while (1) {
		int select;
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

		select = draw_main_menu_sel();

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