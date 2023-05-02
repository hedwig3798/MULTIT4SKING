#include <time.h>
#include <stdio.h>
#include <windows.h>

#include "subgames.h"
#include "screen.h"
#include "definsize.h"
#include "text.h"

#pragma region variables
// 각 서브 보드의 시작지점
extern int g_startingPoint[NUMBER_OF_GAME][2];

// 활성 게임 체크
extern bool activateGame[NUMBER_OF_GAME];

// 게임 보드
extern char subGameBoard[4][S_BOARD_SIZE_ROW][S_BOARD_SIZE_COL + 1];

// 각 게임이 게임 오버가 되었는지 체크
extern bool gameOver[NUMBER_OF_GAME];

// 각 게임의 업데이트 함수 포인터
extern void (*updater[NUMBER_OF_GAME])(void);

// 시간 체크 함수
extern clock_t nowTime, oldTime;

// 튜토리얼 단계
int step = 0;

// 키다운 체크
bool ifKeyDown = true;
#pragma endregion

/// <summary>
/// 튜토리얼 초기화 함수
/// </summary>
void InitTutorial() {
	// 게임 오버 및 게임 활성화 변수 초기화
	for (int i = 0; i < NUMBER_OF_GAME; i++) {
		gameOver[i] = 0;
		activateGame[i] = 0;
	}

	// 스텝 초기화
	step = 0;

	// 튜토리얼 위치 초기화
	for (int i = 0; i < NUMBER_OF_GAME; i++) {
		g_startingPoint[i][0] = (SCREEN_SIZE_Y / 2) - (S_BOARD_SIZE_ROW / 2);
		g_startingPoint[i][1] = (SCREEN_SIZE_X / 2) - (S_BOARD_SIZE_COL / 2);
	}
}

/// <summary>
/// 튜토리얼 업데이트
/// </summary>
int UpdateTutorial() {
	// 튜토리얼 강제 종료
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

	// 튜토리얼 단계에 따라 다음 게임으로 이동
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

	// 활성화 된 게임 업데이트
	for (int i = 0; i < NUMBER_OF_GAME; i++) {
		if (activateGame[i]) {
			updater[i]();
		}
	}

	// 4단계 진입 시 탈출
	if (step == 4) {
		return -1;
	}

	return 1;
}

///튜토리얼 렌더링 함수
void RenderTutorial() {
	// 스크린 초기화
	ScreenClear();

	// 점수 출력
	char tutoText[SCREEN_SIZE_X] = { 0, };
	sprintf_s(tutoText, sizeof(tutoText), "            튜토리얼 에서는 게임 오버가 없습니다!");
	ScreenPrintString(SCREEN_SIZE_X / 2 - 30, 8, tutoText);
	sprintf_s(tutoText, sizeof(tutoText), "     shift 키를 눌러 다음으로 단계로 넘어갈 수 있습니다.");
	ScreenPrintString(SCREEN_SIZE_X / 2 - 30, 9, tutoText);

	// 시작 포인트에 서브 보드 출력
	for (int i = 0; i < NUMBER_OF_GAME; i++) {
		if (activateGame[i]) {
			ScreenPrintString(SCREEN_SIZE_X / 2 - 30, 15, (char *)tutorialText[i]);
		}
	}

	// 시작 포인트에 서브 보드 출력
	for (int i = 0; i < NUMBER_OF_GAME; i++) {
		if (activateGame[i]) {
			for (int si = 0; si < S_BOARD_SIZE_ROW; si++) {
				ScreenPrintString(g_startingPoint[i][1], si + g_startingPoint[i][0], subGameBoard[i][si]);
			}
		}
	}

	// 스크린 플립
	ScreenFlipping();
}

/// <summary>
/// 튜토리얼 종료 함수
/// </summary>
void ReleaseTutorial() {
	// 게임 메모리 반환
	DeleteAllBullet();
	DeleteAllDBullet();
	DeleteAllFBullet();
}