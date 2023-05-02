#include <windows.h>
#include "screen.h"
#include "definsize.h"
#include "text.h"
#include "soundManager.h"
int now_menu = 0;

/// <summary>
/// Ÿ��Ʋ �޴� ���� �� �ʱ�ȭ �Լ�
/// </summary>
void InitTitle() {
	MusicPlay(TITLE_BG);
}

/// <summary>
/// Ÿ��Ʋ �� �޴� ������Ʈ
/// </summary>
/// <returns>� �޴��� ���õǾ� ������</returns>
int UpdateTitle() {
	// Ű �ٿ� üũ ����
	static bool VK_UP_down = false;
	static bool VK_DOWN_down = false;
	static bool VK_SPACE_down = true;

	// Ű �Է� ó��
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

	// �Է� ��
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

	// ���� �޴� ����
	return -1;
}

/// <summary>
/// ���� ȭ�� �� �޴� ������ �Լ�
/// </summary>
void RenderTitle() {
	// ��ũ�� �ʱ�ȭ
	ScreenClear();

	// Ÿ��Ʋ ���
	for (int i = 0; i < 8; i++) {
		ScreenPrintString(10, i + 10, (char*)title[i]);
	}

	// �޴� �� Ŀ�� ���
	for (int i = 0; i < 3; i++) {
		if (now_menu == i) {
			ScreenPrintString(SCREEN_SIZE_X / 2 - 5, i * 3 + SCREEN_SIZE_Y / 2, cursor);
		}
		for (int j = 0; j < 1; j++) {
			ScreenPrintString(SCREEN_SIZE_X / 2 - 2, i * 3 + SCREEN_SIZE_Y / 2 + j, (char*)selector[i][j]);
		}
	}

	// ��ũ�� �ø�
	ScreenFlipping();
}

/// <summary>
/// ���� ȭ�� ��� �� �����ϴ� �Լ�
/// </summary>
void ReleaseTitle() {
	MusicStop();
}