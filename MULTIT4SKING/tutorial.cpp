#include <time.h>
#include <stdio.h>
#include <windows.h>

#include "subgames.h"
#include "screen.h"
#include "definsize.h"
#include "text.h"

#pragma region variables
// �� ���� ������ ��������
extern int g_startingPoint[NUMBER_OF_GAME][2];

// Ȱ�� ���� üũ
extern bool activateGame[NUMBER_OF_GAME];

// ���� ����
extern char subGameBoard[4][S_BOARD_SIZE_ROW][S_BOARD_SIZE_COL + 1];

// �� ������ ���� ������ �Ǿ����� üũ
extern bool gameOver[NUMBER_OF_GAME];

// �� ������ ������Ʈ �Լ� ������
extern void (*updater[NUMBER_OF_GAME])(void);

// �ð� üũ �Լ�
extern clock_t nowTime, oldTime;

// Ʃ�丮�� �ܰ�
int step = 0;

// Ű�ٿ� üũ
bool ifKeyDown = true;
#pragma endregion

/// <summary>
/// Ʃ�丮�� �ʱ�ȭ �Լ�
/// </summary>
void InitTutorial() {
	// ���� ���� �� ���� Ȱ��ȭ ���� �ʱ�ȭ
	for (int i = 0; i < NUMBER_OF_GAME; i++) {
		gameOver[i] = 0;
		activateGame[i] = 0;
	}

	// ���� �ʱ�ȭ
	step = 0;

	// Ʃ�丮�� ��ġ �ʱ�ȭ
	for (int i = 0; i < NUMBER_OF_GAME; i++) {
		g_startingPoint[i][0] = (SCREEN_SIZE_Y / 2) - (S_BOARD_SIZE_ROW / 2);
		g_startingPoint[i][1] = (SCREEN_SIZE_X / 2) - (S_BOARD_SIZE_COL / 2);
	}
}

/// <summary>
/// Ʃ�丮�� ������Ʈ
/// </summary>
int UpdateTutorial() {
	// Ʃ�丮�� ���� ����
	if (GetAsyncKeyState(VK_ESCAPE)) {
		step = 4;
	}

	if (GetAsyncKeyState(VK_SHIFT)) {
		if (!ifKeyDown) {
			ifKeyDown = true;
			step++;
		}
	}
	else {
		ifKeyDown = false;
	}

	// Ʃ�丮�� �ܰ迡 ���� ���� �������� �̵�
	if (step == 0 && !activateGame[0]) {
		PongInit();
		activateGame[0] = true;
	}
	if (step == 1 && !activateGame[1]) {
		DodgeInit();
		activateGame[1] = true;
		activateGame[0] = false;
	}
	if (step == 2 && !activateGame[2]) {
		DefenceInit();
		activateGame[2] = true;
		activateGame[1] = false;
	}
	if (step == 3 && !activateGame[3]) {
		FliperInit();
		activateGame[3] = true;
		activateGame[2] = false;
	}

	// Ȱ��ȭ �� ���� ������Ʈ
	for (int i = 0; i < NUMBER_OF_GAME; i++) {
		if (activateGame[i]) {
			updater[i]();
		}
	}

	// 4�ܰ� ���� �� Ż��
	if (step == 4) {
		return -1;
	}

	return 1;
}

///Ʃ�丮�� ������ �Լ�
void RenderTutorial() {
	// ��ũ�� �ʱ�ȭ
	ScreenClear();

	// ���� ���
	char tutoText[SCREEN_SIZE_X] = { 0, };
	sprintf_s(tutoText, sizeof(tutoText), "            Ʃ�丮�� ������ ���� ������ �����ϴ�!");
	ScreenPrintString(SCREEN_SIZE_X / 2 - 30, 8, tutoText);
	sprintf_s(tutoText, sizeof(tutoText), "     shift Ű�� ���� �������� �ܰ�� �Ѿ �� �ֽ��ϴ�.");
	ScreenPrintString(SCREEN_SIZE_X / 2 - 30, 9, tutoText);

	// ���� ����Ʈ�� ���� ���� ���
	for (int i = 0; i < NUMBER_OF_GAME; i++) {
		if (activateGame[i]) {
			ScreenPrintString(SCREEN_SIZE_X / 2 - 30, 15, (char *)tutorialText[i]);
		}
	}

	// ���� ����Ʈ�� ���� ���� ���
	for (int i = 0; i < NUMBER_OF_GAME; i++) {
		if (activateGame[i]) {
			for (int si = 0; si < S_BOARD_SIZE_ROW; si++) {
				ScreenPrintString(g_startingPoint[i][1], si + g_startingPoint[i][0], subGameBoard[i][si]);
			}
		}
	}

	// ��ũ�� �ø�
	ScreenFlipping();
}

/// <summary>
/// Ʃ�丮�� ���� �Լ�
/// </summary>
void ReleaseTutorial() {
	// ���� �޸� ��ȯ
	DeleteAllBullet();
	DeleteAllDBullet();
	DeleteAllFBullet();
}