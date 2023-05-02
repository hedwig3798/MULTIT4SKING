///
/// ������Ʈ �̸�		: MULTIT4SKING
/// ���� �Ͻ�		: 2023/04/17
/// �ֱ� �۾� ����	: 2023/04/27
/// ����				: ������Ÿ�� (2023/04/25)
///					: 1.1v (2023/04/27)
/// �۾���			: ����ȯ (hedwig)
/// ���� ����		: �ܼ��� ���� 4������ ���ÿ� �����Ͽ� ���� ��Ƽ�� ����
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

// ���� ���� üũ
extern bool gameOver[NUMBER_OF_GAME];
// ���� ���� ����
int gameState = -1;

/// <summary>
/// ���� ���� ����
/// </summary>
int main() {
	// ��ũ�� �ʱ�ȭ
	ScreenInit();

	// ���� �ʱ�ȭ
	SoundInit();

	// �� �ʱ�ȭ
	SetColor(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

	// ������ ��� ���� �ð� ����
	clock_t startTime = clock();
	// 60 ������
	int timeInterval = 1000 / 60;

	// ���� ȭ�� ����
	InitTitle();

	// ���� ���� ����
	while (1) {
		// ������ ��� ���� �ð� ����
		clock_t currentTime = clock();
		int elapsedTime = (int)(currentTime - startTime) * 1000 / CLOCKS_PER_SEC;

		// 60 ������ ����
		if (elapsedTime >= timeInterval) {
			// Ÿ��Ʋ ȭ��
			if (gameState == -1) {
				//������ ����
				gameState = UpdateTitle();
				//ȭ�� ���
				RenderTitle();

				// ���� ���� ó��
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

			// ���� ���� ����
			if (gameState == 0) {
				//������ ����
				UpdateGame();
				//ȭ�� ���
				RenderGame();

				// ���� ������
				if (gameOver[0] || gameOver[2] || gameOver[3] || gameOver[1]) {
					// ���� ���� ����
					gameState = 3;

					//���� �޸� ����
					ReleaseGame();

					// ���� ���� �ʱ�ȭ
					InitOver();
				}
			}

			// Ʃ�丮�� ����
			if (gameState == 1) {
				gameState = UpdateTutorial();
				RenderTutorial();
				if (gameState == -1) {
					ReleaseTutorial();
					InitTitle();
				}
			}

			// ���� ���� ȭ��
			if (gameState == 3) {
				gameState = UpdateOver();
				RenderOver();
				if (gameState == -1) {
					ReleaseOver();
					InitTitle();
				}
			}

			// ���� �ð��� ���� �ð����� ����
			startTime = currentTime;
		}
	}

	// ��ũ�� ����
	ScreenRelease();
	// ���� ����
	ReleaseSound();
	return 0;
}