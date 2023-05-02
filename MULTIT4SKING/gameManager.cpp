#include <time.h>
#include <stdio.h>
#include <windows.h>

#include "definsize.h"
#include "subgames.h"
#include "screen.h"
#include "text.h"
#include "gameManager.h"
#include "alphabet.h"
#include "leaderboard.h"
#include "soundManager.h"

/// ���� ���� ����
#pragma region variables
// �� ���� ������ ��������
int g_startingPoint[NUMBER_OF_GAME][2] = { 0, };

// Ȱ�� ���� üũ
bool activateGame[NUMBER_OF_GAME] = { 0, };

// ����, ��ƾ �ð�(��)
int score = 0;

// ���� ����
char subGameBoard[4][S_BOARD_SIZE_ROW][S_BOARD_SIZE_COL + 1];

// �� ������ ���� ������ �Ǿ����� üũ
bool gameOver[NUMBER_OF_GAME];

// �� ������ ������Ʈ �Լ� ������
void (*updater[NUMBER_OF_GAME])(void);

// ���� ������ �̸� �Է��� �ޱ� ���� ����
char inputName[NAME_LENGHT] = { 0, };

// �̸��� �ε���
char inputNameIndex = 0;

// Ű �ٿ� ������ ���� �� ����
bool ifKeyDown[4] = { 0, };

// �ð� üũ �Լ�
clock_t nowTime, oldTime;
#pragma endregion

/// <summary>
/// ���� �ʱ�ȭ
/// �� ���Ӻ� �ʱ�ȭ �Լ� ����
/// </summary>
void InitGames() {
	// ���� ���� �� ���� Ȱ��ȭ ���� �ʱ�ȭ
	for (int i = 0; i < NUMBER_OF_GAME; i++) {
		gameOver[i] = 0;
		activateGame[i] = 0;
	}

	// ���� �ʱ�ȭ
	score = 0;

	// ���ھ� üũ�� �ð� üũ ���� �ʱ�ȭ
	nowTime = clock();
	oldTime = clock();

	MusicPlay(INGAME_BG);
}

/// <summary>
/// ���� ���� ȭ�� �ʱ�ȭ
/// </summary>
void InitOver() {
	// �������� �ҷ�����
	int success = GetLeaderBoard();
	EffectPlay(GAME_OVER_EF);
	Sleep(3000);
	MusicPlay(END_BG);
}

/// <summary>
/// ���� ƽ ���� ����Ǿ����� �Լ�
/// </summary>
void UpdateGame() {
	// ���ھ� üũ (1�ʴ� 1��)
	nowTime = clock();
	if (nowTime - oldTime >= 1000) {
		score++;
		oldTime = nowTime;
	}

	// ���� �ð��� ���� �� ���� ���ο� ���� �߰�
	if (score >= (INTERVAL * 0) && !activateGame[0]) {
		PongInit();
		g_startingPoint[0][0] = (SCREEN_SIZE_Y / 2) - (S_BOARD_SIZE_ROW / 2);
		g_startingPoint[0][1] = (SCREEN_SIZE_X / 2) - (S_BOARD_SIZE_COL / 2);
		activateGame[0] = true;
	}
	if (score >= (INTERVAL * 1) && !activateGame[1]) {
		DodgeInit();
		g_startingPoint[1][0] = (SCREEN_SIZE_Y / 2) - (S_BOARD_SIZE_ROW / 2);
		g_startingPoint[1][1] = (SCREEN_SIZE_X / 2) - (S_BOARD_SIZE_COL / 2);

		g_startingPoint[0][1] -= (S_BOARD_SIZE_COL / 2);
		g_startingPoint[1][1] += (S_BOARD_SIZE_COL / 2) - 1;
		activateGame[1] = true;
		EffectPlay(GAME_CREATE_EF);
	}
	if (score >= (INTERVAL * 2) && !activateGame[2]) {
		DefenceInit();
		g_startingPoint[2][0] = (SCREEN_SIZE_Y / 2) - (S_BOARD_SIZE_ROW / 2);
		g_startingPoint[2][1] = (SCREEN_SIZE_X / 2) - (S_BOARD_SIZE_COL / 2);

		g_startingPoint[0][0] -= (S_BOARD_SIZE_ROW / 2) - 1;
		g_startingPoint[1][0] -= (S_BOARD_SIZE_ROW / 2) - 1;
		g_startingPoint[2][0] += (S_BOARD_SIZE_ROW / 2);

		activateGame[2] = true;
		EffectPlay(GAME_CREATE_EF);
	}
	if (score >= (INTERVAL * 3) && !activateGame[3]) {
		FliperInit();
		g_startingPoint[3][0] = (SCREEN_SIZE_Y / 2) - (S_BOARD_SIZE_ROW / 2);
		g_startingPoint[3][1] = (SCREEN_SIZE_X / 2) - (S_BOARD_SIZE_COL / 2);

		g_startingPoint[2][1] -= (S_BOARD_SIZE_COL / 2);
		g_startingPoint[3][1] += (S_BOARD_SIZE_COL / 2) - 1;
		g_startingPoint[3][0] += (S_BOARD_SIZE_ROW / 2);

		activateGame[3] = true;
		EffectPlay(GAME_CREATE_EF);
	}

	// Ȱ��ȭ �� ���� ������Ʈ
	for (int i = 0; i < NUMBER_OF_GAME; i++) {
		if (activateGame[i]) {
			updater[i]();
		}
	}
}

/// <summary>
/// ���� ���� ȭ�鿡�� �� ƽ ���� ����Ǿ����� �Լ�
/// </summary>
int UpdateOver() {
	// Ű �Է¿� ���� �̸� �Է�
	if (GetAsyncKeyState(VK_UP)) {
		if (!ifKeyDown[0]) {
			inputName[inputNameIndex]++;
			if (inputName[inputNameIndex] > 25) {
				inputName[inputNameIndex] = 0;
			}
			ifKeyDown[0] = true;
		}
	}
	else {
		ifKeyDown[0] = false;
	}

	if (GetAsyncKeyState(VK_DOWN)) {
		if (!ifKeyDown[1]) {
			inputName[inputNameIndex]--;
			if (inputName[inputNameIndex] < 0) {
				inputName[inputNameIndex] = 25;
			}
			ifKeyDown[1] = true;
		}
	}
	else {
		ifKeyDown[1] = false;
	}

	if (GetAsyncKeyState(VK_LEFT)) {
		if (!ifKeyDown[2]) {
			inputNameIndex--;
			if (inputNameIndex < 0) {
				inputNameIndex = 2;
			}
			ifKeyDown[2] = true;
		}
	}
	else {
		ifKeyDown[2] = false;
	}

	if (GetAsyncKeyState(VK_RIGHT)) {
		if (!ifKeyDown[3]) {
			inputNameIndex++;
			if (inputNameIndex > 2) {
				inputNameIndex = 0;
			}
			ifKeyDown[3] = true;
		}
	}
	else {
		ifKeyDown[3] = false;
	}

	// �����̽��� ������ �� ���ھ� ���� �� ���θ޴��� �̵�
	if (GetAsyncKeyState(VK_SPACE)) {
		Player p;
		for (int i = 0; i < NAME_LENGHT; i++) {
			p.name[i] = inputName[i] + 'A';
		}
		p.name[3] = '\0';
		p.score = score;

		players[playerCount] = p;
		SetLeaderBoard();
		return -1;
	}

	// �� �� �ݺ�
	return 3;
}

/// <summary>
/// ���� ȭ�� ������ �Լ�
/// </summary>
void RenderGame() {
	// ��ũ�� �ʱ�ȭ
	ScreenClear();

	// ���� ���
	char scoreBoard[SCREEN_SIZE_X] = { 0, };
	sprintf_s(scoreBoard, sizeof(scoreBoard), "Score : %d", score);
	ScreenPrintString(SCREEN_SIZE_X / 2 - 5, 5, scoreBoard);

	// ���� ����Ʈ�� ���� ���� ���
	for (int i = 0; i < NUMBER_OF_GAME; i++) {
		if (activateGame[i]) {
			for (int si = 0; si < S_BOARD_SIZE_ROW; si++) {
				ScreenPrintString(g_startingPoint[i][1], si + g_startingPoint[i][0], subGameBoard[i][si]);
			}
		}
	}
	SetColor(FOREGROUND_INTENSITY);
	for (int i = 0; i < NUMBER_OF_GAME; i++) {
		if (gameOver[i]) {
			for (int si = 0; si < S_BOARD_SIZE_ROW; si++) {
				ScreenPrintString(g_startingPoint[i][1], si + g_startingPoint[i][0], subGameBoard[i][si]);
			}
		}
	}
	SetColor(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

	// ��ũ�� �ø�
	ScreenFlipping();
}

/// <summary>
/// ���� ���� ȭ�� ������ �Լ�
/// </summary>
void RenderOver() {
	// ��ũ�� �ʱ�ȭ
	ScreenClear();

	// ���� ���� �ؽ�Ʈ ���
	for (int i = 0; i < 7; i++) {
		ScreenPrintString(SCREEN_SIZE_X / 2 - 40, SCREEN_SIZE_Y / 2 + i - 15, (char*)gameOverText[i]);
	}

	// �������� ���
	for (int i = 0; i < playerCount && i < MAX_USER; i++) {
		char leaderBoard[SCREEN_SIZE_X] = { 0, };
		sprintf_s(leaderBoard, sizeof(leaderBoard), "%d: %s %d", i + 1, players[i].name, players[i].score);
		ScreenPrintString(20, SCREEN_SIZE_Y / 2 + i - (MAX_USER / 2), leaderBoard);
	}

	// �̸� �Է� �ȳ��� ���
	char insertText[] = "Insert Name";
	ScreenPrintString(SCREEN_SIZE_X / 2 - 6, SCREEN_SIZE_Y / 2 - 5, insertText);

	// �޼� ���ھ� ���
	char scoreBoard[SCREEN_SIZE_X] = { 0, };
	sprintf_s(scoreBoard, sizeof(scoreBoard), "Score : %d", score);
	ScreenPrintString(SCREEN_SIZE_X / 2 - 5, 5, scoreBoard);

	// �̸� �Է� ĭ ���
	for (int i = 0; i < NAME_LENGHT; i++) {
		for (int j = 0; j < APLHA_SIZE_ROW; j++) {
			ScreenPrintString(SCREEN_SIZE_X / 2 + i * (APLHA_SIZE_COL + 10) - 25, SCREEN_SIZE_Y / 2 + j, (char*)alphabet[inputName[i]][j]);
		}
		ScreenPrintString(SCREEN_SIZE_X / 2 + inputNameIndex * (APLHA_SIZE_COL + 10) - 25, SCREEN_SIZE_Y / 2 + APLHA_SIZE_ROW, (char*)underbar);
	}

	// ��ũ�� �ø�
	ScreenFlipping();
}

/// <summary>
/// ���� ����� ����Ǵ� �Լ�
/// �� ������ �޸� ��ȯ �Լ� ����
/// </summary>
void ReleaseGame() {
	// ���� �޸� ��ȯ
	DeleteAllBullet();
	DeleteAllDBullet();
	DeleteAllFBullet();
	
	// ���� ����
	MusicStop();
}

/// <summary>
/// ���� ���� ȭ�� ��� �� ����Ǵ� �Լ�
/// </summary>
void ReleaseOver() { 
	MusicStop();
}