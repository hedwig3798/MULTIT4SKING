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

/// 전역 변수 모음
#pragma region variables
// 각 서브 보드의 시작지점
int g_startingPoint[NUMBER_OF_GAME][2] = { 0, };

// 활성 게임 체크
bool activateGame[NUMBER_OF_GAME] = { 0, };

// 점수, 버틴 시간(초)
int score = 0;

// 게임 보드
char subGameBoard[4][S_BOARD_SIZE_ROW][S_BOARD_SIZE_COL + 1];

// 각 게임이 게임 오버가 되었는지 체크
bool gameOver[NUMBER_OF_GAME];

// 각 게임의 업데이트 함수 포인터
void (*updater[NUMBER_OF_GAME])(void);

// 게임 오버시 이름 입력을 받기 위한 변수
char inputName[NAME_LENGHT] = { 0, };

// 이름의 인덱스
char inputNameIndex = 0;

// 키 다운 감지를 위한 불 변수
bool ifKeyDown[4] = { 0, };

// 시간 체크 함수
clock_t nowTime, oldTime;
#pragma endregion

/// <summary>
/// 게임 초기화
/// 각 게임별 초기화 함수 실행
/// </summary>
void InitGames() {
	// 게임 오버 및 게임 활성화 변수 초기화
	for (int i = 0; i < NUMBER_OF_GAME; i++) {
		gameOver[i] = 0;
		activateGame[i] = 0;
	}

	// 점수 초기화
	score = 0;

	// 스코어 체크용 시간 체크 변수 초기화
	nowTime = clock();
	oldTime = clock();

	MusicPlay(INGAME_BG);
}

/// <summary>
/// 게임 오버 화면 초기화
/// </summary>
void InitOver() {
	// 리더보드 불러오기
	int success = GetLeaderBoard();
	EffectPlay(GAME_OVER_EF);
	Sleep(3000);
	MusicPlay(END_BG);
}

/// <summary>
/// 게임 틱 마다 실행되어지는 함수
/// </summary>
void UpdateGame() {
	// 스코어 체크 (1초당 1점)
	nowTime = clock();
	if (nowTime - oldTime >= 1000) {
		score++;
		oldTime = nowTime;
	}

	// 일정 시간이 지날 때 마다 새로운 게임 추가
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

	// 활성화 된 게임 업데이트
	for (int i = 0; i < NUMBER_OF_GAME; i++) {
		if (activateGame[i]) {
			updater[i]();
		}
	}
}

/// <summary>
/// 게임 오버 화면에서 매 틱 마다 실행되어지는 함수
/// </summary>
int UpdateOver() {
	// 키 입력에 따른 이름 입력
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

	// 스페이스바 눌렀을 시 스코어 저장 및 메인메뉴로 이동
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

	// 그 외 반복
	return 3;
}

/// <summary>
/// 게임 화면 렌더링 함수
/// </summary>
void RenderGame() {
	// 스크린 초기화
	ScreenClear();

	// 점수 출력
	char scoreBoard[SCREEN_SIZE_X] = { 0, };
	sprintf_s(scoreBoard, sizeof(scoreBoard), "Score : %d", score);
	ScreenPrintString(SCREEN_SIZE_X / 2 - 5, 5, scoreBoard);

	// 시작 포인트에 서브 보드 출력
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

	// 스크린 플립
	ScreenFlipping();
}

/// <summary>
/// 게임 오버 화면 렌더링 함수
/// </summary>
void RenderOver() {
	// 스크린 초기화
	ScreenClear();

	// 게임 오버 텍스트 출력
	for (int i = 0; i < 7; i++) {
		ScreenPrintString(SCREEN_SIZE_X / 2 - 40, SCREEN_SIZE_Y / 2 + i - 15, (char*)gameOverText[i]);
	}

	// 리더보드 출력
	for (int i = 0; i < playerCount && i < MAX_USER; i++) {
		char leaderBoard[SCREEN_SIZE_X] = { 0, };
		sprintf_s(leaderBoard, sizeof(leaderBoard), "%d: %s %d", i + 1, players[i].name, players[i].score);
		ScreenPrintString(20, SCREEN_SIZE_Y / 2 + i - (MAX_USER / 2), leaderBoard);
	}

	// 이름 입력 안내문 출력
	char insertText[] = "Insert Name";
	ScreenPrintString(SCREEN_SIZE_X / 2 - 6, SCREEN_SIZE_Y / 2 - 5, insertText);

	// 달성 스코어 출력
	char scoreBoard[SCREEN_SIZE_X] = { 0, };
	sprintf_s(scoreBoard, sizeof(scoreBoard), "Score : %d", score);
	ScreenPrintString(SCREEN_SIZE_X / 2 - 5, 5, scoreBoard);

	// 이름 입력 칸 출력
	for (int i = 0; i < NAME_LENGHT; i++) {
		for (int j = 0; j < APLHA_SIZE_ROW; j++) {
			ScreenPrintString(SCREEN_SIZE_X / 2 + i * (APLHA_SIZE_COL + 10) - 25, SCREEN_SIZE_Y / 2 + j, (char*)alphabet[inputName[i]][j]);
		}
		ScreenPrintString(SCREEN_SIZE_X / 2 + inputNameIndex * (APLHA_SIZE_COL + 10) - 25, SCREEN_SIZE_Y / 2 + APLHA_SIZE_ROW, (char*)underbar);
	}

	// 스크린 플립
	ScreenFlipping();
}

/// <summary>
/// 게임 종료시 실행되는 함수
/// 각 게임의 메모리 반환 함수 실행
/// </summary>
void ReleaseGame() {
	// 게임 메모리 반환
	DeleteAllBullet();
	DeleteAllDBullet();
	DeleteAllFBullet();
	
	// 음악 종료
	MusicStop();
}

/// <summary>
/// 게임 오버 화면 벗어날 때 실행되는 함수
/// </summary>
void ReleaseOver() { 
	MusicStop();
}