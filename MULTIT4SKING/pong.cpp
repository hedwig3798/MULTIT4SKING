#include "definsize.h"
#include<stdio.h>
#include <windows.h>
#include <math.h>
#include "boardManager.h"
#include "soundManager.h"
/// 구조체 모음
#pragma region structs
// 플레이어가 조작하는 오브젝트 변수
struct PongPlayer {
	double position[2] = { 0, 0 };
	int sizeX = 7;
	int sizeY = 1;
	char character = '=';
	double speed = 0.5;
};

// 공 오브젝트
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

/// 전역 변수 모음
#pragma region variable
// 서브 게임 보드
extern char subGameBoard[4][S_BOARD_SIZE_ROW][S_BOARD_SIZE_COL + 1];

// 업데이터
extern void (*updater[NUMBER_OF_GAME])(void);

// 플레이어 오브젝트
PongPlayer pongPlayer;

// 공 오브젝트
PongBall pongBall;
#pragma endregion

/// 함수 선언 모음
#pragma region funcs
// 함수 선언
void PongInit();
void PongUpdate();
#pragma endregion

/// <summary>
/// 초기화 함수
/// </summary>
void PongInit() {
	// 게임 ID
	int gameID = 0;

	// 업데이터 함수 전달
	updater[gameID] = PongUpdate;

	// 보드 초기화
	InitBoard(subGameBoard[gameID]);

	// 플레이어 초기화
	pongPlayer = PongPlayer();
	pongPlayer.position[0] = S_BOARD_SIZE_ROW - 4;
	pongPlayer.position[1] = (S_BOARD_SIZE_COL / 2) - (pongPlayer.sizeX / 2);

	// 공 초기화
	pongBall = PongBall();

	// 공 시작 위치
	pongBall.position[0] = (int)(S_BOARD_SIZE_ROW / 2);
	pongBall.position[1] = S_BOARD_SIZE_COL / 2;

	// 공 초기 움직임 벡터
	pongBall.direction[0] = 1;
	pongBall.direction[1] = 1;
	pongBall.angle = M_PI / 3;
}

/// <summary>
/// 업데이트 함수
/// </summary>
void PongUpdate() {
	// 게임 ID
	int gameID = 0;

	// 플레이어 이동
	if (pongPlayer.position[1] > 2 && GetAsyncKeyState(VK_LEFT)) {
		pongPlayer.position[1] -= pongPlayer.speed;
	}
	if (pongPlayer.position[1] < S_BOARD_SIZE_COL-2-pongPlayer.sizeX && GetAsyncKeyState(VK_RIGHT)) {
		pongPlayer.position[1] += pongPlayer.speed;
	}

	pongBall.position[0] -= sin(pongBall.angle) * pongBall.speed * pongBall.direction[0];
	pongBall.position[1] -= cos(pongBall.angle) * pongBall.speed * pongBall.direction[1];

	// 벽에 닿았을 경우 처리
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

	// 플레이어에 닿았을 경우 처리
	if ((int)(pongBall.position[0]) <= (int)(pongPlayer.position[0]) && (int)(pongBall.position[0]) > (int)(pongPlayer.position[0]-1)) {
		if (pongBall.position[1] >= pongPlayer.position[1] && pongBall.position[1] <= pongPlayer.position[1] + pongPlayer.sizeX) {
			pongBall.direction[0] = 1;
			EffectPlay(PONG_EF);
		}
	}

	// 보드 초기화
	InitBoard(subGameBoard[gameID]);
	
	// 플레이어 출력
	DrawObjectAtBoard(subGameBoard[gameID], pongPlayer.character, pongPlayer.sizeX, pongPlayer.sizeY, (int)pongPlayer.position[0], (int)pongPlayer.position[1]);
	
	// 공 출력
	DrawObjectAtBoard(subGameBoard[gameID], pongBall.character, pongBall.sizeX, pongBall.sizeY, (int)pongBall.position[0], (int)pongBall.position[1]);
}
