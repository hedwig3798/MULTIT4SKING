///
/// 프로젝트 이름		: MULTIT4SKING
/// 시작 일시		: 2023/04/17
/// 최근 작업 일자	: 2023/04/27
/// 버전				: 프로토타입 (2023/04/25)
///					: 1.1v (2023/04/27)
/// 작업자			: 김형환 (hedwig)
/// 한줄 설명		: 단순한 게임 4가지를 동시에 진행하여 오래 버티는 게임
/// 
#include<stdio.h>
#include<time.h>
#include <stdlib.h>
#include <windows.h>
#include "screen.h"
#include "definsize.h"
#include "subgames.h"
#include "text.h"
#include "titleManager.h"
#include "gameManager.h"
#include "tutorial.h"
#include "soundManager.h"

// 게임 오버 체크
extern bool gameOver[NUMBER_OF_GAME];
// 게임 상태 변수
int gameState = -1;

/// <summary>
/// 게임 메인 로직
/// </summary>
int main() {
	// 스크린 초기화
	ScreenInit();

	// 사운드 초기화
	SoundInit();

	// 색 초기화
	SetColor(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

	// 프레임 제어를 위한 시간 변수
	clock_t startTime = clock();
	// 60 프레임
	int timeInterval = 1000 / 60;

	// 메인 화면 시작
	InitTitle();

	// 메인 게임 루프
	while (1) {
		// 프레임 제어를 위한 시간 변수
		clock_t currentTime = clock();
		int elapsedTime = (int)(currentTime - startTime) * 1000 / CLOCKS_PER_SEC;

		// 60 프레임 고정
		if (elapsedTime >= timeInterval) {
			// 타이틀 화면
			if (gameState == -1) {
				//데이터 갱신
				gameState = UpdateTitle();
				//화면 출력
				RenderTitle();

				// 게임 상태 처리
				if (gameState != -1) {
					ReleaseTitle();
				}
				if (gameState == 0) {
					InitGames();
				}
				else if (gameState == 2) {
					return 0;
				}
				else if (gameState == 1) {
					InitTutorial();
				}
			}

			// 메인 게임 루프
			if (gameState == 0) {
				//데이터 갱신
				UpdateGame();
				//화면 출력
				RenderGame();

				// 게임 오버시
				if (gameOver[0] || gameOver[2] || gameOver[3] || gameOver[1]) {
					// 게임 상태 변경
					gameState = 3;

					//게임 메모리 해제
					ReleaseGame();

					// 게임 오버 초기화
					InitOver();
				}
			}

			// 튜토리얼 루프
			if (gameState == 1) {
				gameState = UpdateTutorial();
				RenderTutorial();
				if (gameState == -1) {
					ReleaseTutorial();
					InitTitle();
				}
			}

			// 게임 오버 화면
			if (gameState == 3) {
				gameState = UpdateOver();
				RenderOver();
				if (gameState == -1) {
					ReleaseOver();
					InitTitle();
				}
			}

			// 현재 시간을 시작 시간으로 변경
			startTime = currentTime;
		}
	}

	// 스크린 해제
	ScreenRelease();
	// 사운드 해제
	ReleaseSound();
	return 0;
}