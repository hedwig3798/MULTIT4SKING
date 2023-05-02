#include "definsize.h"
#include<stdio.h>
#include <windows.h>
#include <math.h>
#include "boardManager.h"
#include "soundManager.h"
/// ����ü ����
#pragma region structs
// �÷��̾ �����ϴ� ������Ʈ ����
struct PongPlayer {
	double position[2] = { 0, 0 };
	int sizeX = 7;
	int sizeY = 1;
	char character = '=';
	double speed = 0.5;
};

// �� ������Ʈ
struct PongBall {
	double position[2] = { 0, 0 };
	int sizeX = 1;
	int sizeY = 1;
	char character = 'O';
	int direction[2] = { 1, 1 };
	double angle = 0.0;
	double speed = 0.1;
};
#pragma endregion

/// ���� ���� ����
#pragma region variable
// ���� ���� ����
extern char subGameBoard[4][S_BOARD_SIZE_ROW][S_BOARD_SIZE_COL + 1];

// ��������
extern void (*updater[NUMBER_OF_GAME])(void);

// �÷��̾� ������Ʈ
PongPlayer pongPlayer;

// �� ������Ʈ
PongBall pongBall;
#pragma endregion

/// �Լ� ���� ����
#pragma region funcs
// �Լ� ����
void PongInit();
void PongUpdate();
#pragma endregion

/// <summary>
/// �ʱ�ȭ �Լ�
/// </summary>
void PongInit() {
	// ���� ID
	int gameID = 0;

	// �������� �Լ� ����
	updater[gameID] = PongUpdate;

	// ���� �ʱ�ȭ
	InitBoard(subGameBoard[gameID]);

	// �÷��̾� �ʱ�ȭ
	pongPlayer = PongPlayer();
	pongPlayer.position[0] = S_BOARD_SIZE_ROW - 4;
	pongPlayer.position[1] = (S_BOARD_SIZE_COL / 2) - (pongPlayer.sizeX / 2);

	// �� �ʱ�ȭ
	pongBall = PongBall();

	// �� ���� ��ġ
	pongBall.position[0] = (int)(S_BOARD_SIZE_ROW / 2);
	pongBall.position[1] = S_BOARD_SIZE_COL / 2;

	// �� �ʱ� ������ ����
	pongBall.direction[0] = 1;
	pongBall.direction[1] = 1;
	pongBall.angle = M_PI / 3;
}

/// <summary>
/// ������Ʈ �Լ�
/// </summary>
void PongUpdate() {
	// ���� ID
	int gameID = 0;

	// �÷��̾� �̵�
	if (pongPlayer.position[1] > 2 && GetAsyncKeyState(VK_LEFT)) {
		pongPlayer.position[1] -= pongPlayer.speed;
	}
	if (pongPlayer.position[1] < S_BOARD_SIZE_COL-2-pongPlayer.sizeX && GetAsyncKeyState(VK_RIGHT)) {
		pongPlayer.position[1] += pongPlayer.speed;
	}

	pongBall.position[0] -= sin(pongBall.angle) * pongBall.speed * pongBall.direction[0];
	pongBall.position[1] -= cos(pongBall.angle) * pongBall.speed * pongBall.direction[1];

	// ���� ����� ��� ó��
	if (pongBall.position[0] <= 1) {
		pongBall.position[0] = 1;
		pongBall.direction[0] = -1;
		EffectPlay(PONG_EF);
	}
	if (pongBall.position[0] > S_BOARD_SIZE_ROW - 2) {
		pongBall.position[0] = S_BOARD_SIZE_ROW - 2;
		pongBall.direction[0] = 1;
		GameOver(gameID);
	}
	if (pongBall.position[1] <= 1) {
		pongBall.position[1] = 1;
		pongBall.direction[1] = -1;
		EffectPlay(PONG_EF);
	}
	if (pongBall.position[1] > S_BOARD_SIZE_COL - 2) {
		pongBall.position[1] = S_BOARD_SIZE_COL - 2;
		pongBall.direction[1] = 1;
		EffectPlay(PONG_EF);
	}

	// �÷��̾ ����� ��� ó��
	if ((int)(pongBall.position[0]) <= (int)(pongPlayer.position[0]) && (int)(pongBall.position[0]) > (int)(pongPlayer.position[0]-1)) {
		if (pongBall.position[1] >= pongPlayer.position[1] && pongBall.position[1] <= pongPlayer.position[1] + pongPlayer.sizeX) {
			pongBall.direction[0] = 1;
			EffectPlay(PONG_EF);
		}
	}

	// ���� �ʱ�ȭ
	InitBoard(subGameBoard[gameID]);
	
	// �÷��̾� ���
	DrawObjectAtBoard(subGameBoard[gameID], pongPlayer.character, pongPlayer.sizeX, pongPlayer.sizeY, (int)pongPlayer.position[0], (int)pongPlayer.position[1]);
	
	// �� ���
	DrawObjectAtBoard(subGameBoard[gameID], pongBall.character, pongBall.sizeX, pongBall.sizeY, (int)pongBall.position[0], (int)pongBall.position[1]);
}
