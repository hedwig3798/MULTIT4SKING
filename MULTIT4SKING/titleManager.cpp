#include <windows.h>
#include "screen.h"
#include "definsize.h"
#include "text.h"
#include "soundManager.h"
int now_menu = 0;

/// <summary>
/// 타이틀 메뉴 시작 시 초기화 함수
/// </summary>
void InitTitle() {
	MusicPlay(TITLE_BG);
}

/// <summary>
/// 타이틀 및 메뉴 업데이트
/// </summary>
/// <returns>어떤 메뉴가 선택되어 졌는지</returns>
int UpdateTitle() {
	// 키 다운 체크 변수
	static bool VK_UP_down = false;
	static bool VK_DOWN_down = false;
	static bool VK_SPACE_down = true;

	// 키 입력 처리
	if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState(VK_W)) {
		if (!VK_UP_down) {
			now_menu--;
			if (now_menu < 0) {
				now_menu = 0;
			}
			VK_UP_down = true;
		}
	}
	else {
		VK_UP_down = false;
	}
	if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState(VK_S)) {
		if (!VK_DOWN_down) {
			now_menu++;
			if (now_menu > 2) {
				now_menu = 2;
			}
			VK_DOWN_down = true;
		}
	}
	else {
		VK_DOWN_down = false;
	}

	// 입력 시
	if (GetAsyncKeyState(VK_SPACE) || GetAsyncKeyState(VK_RETURN)) {
		EffectPlay(SELECT_EF);
		if (!VK_SPACE_down) {
			VK_SPACE_down = true;
			switch (now_menu) {
			case 0:
				return 0;
				break;
			case 1:
				return 1;
				break;
			case 2:
				return 2;
				break;
			default:
				break;
			}
		}
	}
	else {
		VK_SPACE_down = false;
	}

	// 메인 메뉴 리턴
	return -1;
}

/// <summary>
/// 메인 화면 및 메뉴 렌더링 함수
/// </summary>
void RenderTitle() {
	// 스크린 초기화
	ScreenClear();

	// 타이틀 출력
	for (int i = 0; i < 8; i++) {
		ScreenPrintString(10, i + 10, (char*)title[i]);
	}

	// 메뉴 및 커서 출력
	for (int i = 0; i < 3; i++) {
		if (now_menu == i) {
			ScreenPrintString(SCREEN_SIZE_X / 2 - 5, i * 3 + SCREEN_SIZE_Y / 2, cursor);
		}
		for (int j = 0; j < 1; j++) {
			ScreenPrintString(SCREEN_SIZE_X / 2 - 2, i * 3 + SCREEN_SIZE_Y / 2 + j, (char*)selector[i][j]);
		}
	}

	// 스크린 플립
	ScreenFlipping();
}

/// <summary>
/// 메인 화면 벗어날 시 실행하는 함수
/// </summary>
void ReleaseTitle() {
	MusicStop();
}