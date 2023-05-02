#include<stdio.h>
#include<time.h>
#include<malloc.h>
#include <windows.h>
#include "definsize.h"
#include "boardManager.h"
#include "soundManager.h"

/// ����ü ����
#pragma region structes
// �÷��̾� ������Ʈ
struct FliperPlayer {
	int position[2] = { 0, 0 };
	int sizeX = 1;
	int sizeY = 3;
	char character = '@';
	int speed = 1;
	bool isUp = false;
};

// ���ؼ� ������Ʈ
struct FliperHorizon {
	int position[2] = { 0, 0 };
	int sizeX = S_BOARD_SIZE_COL - 2;
	int sizeY = 1;
	char character = '-';
	int speed = 1;
};

// ���ؾ� �� ������Ʈ
struct FliperBullet {
	int position[2] = { 0, 0 };
	int sizeX = 1;
	int sizeY = 3;
	char character[2] = { 'W', 'M' };
	bool direction = 0;
};

// �Ѿ� ������Ʈ ��ũ ����Ʈ
struct FliperBulletNode {
	FliperBullet bullet;
	FliperBulletNode* next;
};
#pragma endregion

/// ���� ���� ����
#pragma region variables
// ���� ���� ����
extern char subGameBoard[4][S_BOARD_SIZE_ROW][S_BOARD_SIZE_COL + 1];

// �������� �Լ� ���� ������
extern void (*updater[NUMBER_OF_GAME])(void);

// �÷��̾� ������Ʈ
FliperPlayer fliperPlayer;

// ���� ������Ʈ
FliperHorizon fliperHorizon;

// �Ѿ� ������Ʈ
FliperBullet fliperBullet;

// ����Ʈ ��Ʈ �� ��
FliperBulletNode* Froot;
FliperBulletNode* Fend;

// Ű�ٿ� üũ ����
bool keyDown = false;

// �� ����
int createFTerm = 0;
int oldFScore = 0;
int FTerm = 0;
#pragma endregion

/// �Լ� ���� ����
#pragma region funcs
void FliperInit();
void FliperUpdate();
FliperBullet CreateFBullet();
void InsertFBullet(FliperBullet bullet);
void DeleteFBullet(FliperBulletNode* node, FliperBulletNode* pre);
void CheckFBullet();
#pragma endregion

/// <summary>
/// �ʱ�ȭ �Լ�
/// </summary>
void FliperInit() {
	// ���� ID
	int gameID = 3;

	// �������� �Լ� ����
	updater[gameID] = FliperUpdate;

	// ���� �ʱ�ȭ
	InitBoard(subGameBoard[gameID]);

	// �÷��̾� �ʱ�ȭ
	fliperPlayer.position[0] = S_BOARD_SIZE_ROW / 2 + 1;
	fliperPlayer.position[1] = 3;
	fliperPlayer.isUp = false;

	// ���� �ʱ�ȭ
	fliperHorizon.position[0] = S_BOARD_SIZE_ROW / 2;
	fliperHorizon.position[1] = 1;

	// �Ѿ� ����Ʈ �ʱ�ȭ
	Froot = NULL;
	Fend = NULL;

	// �� �ʱ�ȭ
	FTerm = clock();
	createFTerm = FTerm / 500;
	oldFScore = FTerm / 250;
}

/// <summary>
/// ���� ƽ ���� ����Ǵ� �Լ�
/// </summary>
void FliperUpdate() {
	// ���� ���̵�
	int gameID = 3;

	// �� ����
	FTerm = clock();

	// Ű�Է� ó��
	if (GetAsyncKeyState(VK_SPACE)) {
		if (!keyDown) {
			EffectPlay(FLIPER_EF);
			keyDown = true;

			if (fliperPlayer.isUp) {
				fliperPlayer.position[0] += fliperPlayer.sizeY + 1;
			}
			else {
				fliperPlayer.position[0] -= fliperPlayer.sizeY + 1;
			}
			fliperPlayer.isUp = !fliperPlayer.isUp;
		}
	}
	else {
		keyDown = false;
	}

	// ����
	if (createFTerm < FTerm / 500) {
		InsertFBullet(CreateFBullet());
		createFTerm += 7;
	}

	// �̵�
	if (FTerm / 250 > oldFScore) {
		oldFScore += 3;
		CheckFBullet();
	}

	// ���� �ʱ�ȭ
	InitBoard(subGameBoard[gameID]);

	// �÷��̾� ���
	DrawObjectAtBoard(subGameBoard[gameID], fliperPlayer.character, fliperPlayer.sizeX, fliperPlayer.sizeY, fliperPlayer.position[0], fliperPlayer.position[1]);
	
	// ���� ���
	DrawObjectAtBoard(subGameBoard[gameID], fliperHorizon.character, fliperHorizon.sizeX, fliperHorizon.sizeY, fliperHorizon.position[0], fliperHorizon.position[1]);
	
	// �Ѿ� ���
	FliperBulletNode* node = Froot;
	FliperBulletNode* pre = NULL;
	// ���� �÷��̾�� �浹 �� ���� �� ���� ����
	for (; !(node == NULL);) {
		if (node->bullet.position[1] == fliperPlayer.position[1] && node->bullet.direction == !fliperPlayer.isUp) {
			FliperBulletNode* temp = node->next;
			DeleteFBullet(node, pre);
			node = temp;
			GameOver(3);
			continue;
		}
		DrawObjectAtBoard(subGameBoard[gameID], node->bullet.character[!node->bullet.direction], node->bullet.sizeX, node->bullet.sizeY, node->bullet.position[0], node->bullet.position[1]);
		pre = node;
		node = node->next;
	}
}

/// <summary>
/// �Ѿ� ���� �Լ�
/// </summary>
/// <returns>���� �� �Ѿ�</returns>
FliperBullet CreateFBullet() {
	// ���� �õ� �ʱ�ȭ
	srand(time(NULL));

	// �Ѿ� ����
	FliperBullet bullet;

	// ���� ����
	bullet.direction = rand() % 2;

	// ���⿡ ���� ��ġ ����
	if (bullet.direction) {
		bullet.position[0] = fliperHorizon.position[0] + 1;
		bullet.position[1] = S_BOARD_SIZE_COL - 1;
	}
	else {
		bullet.position[0] = fliperHorizon.position[0] - bullet.sizeY;
		bullet.position[1] = S_BOARD_SIZE_COL - 1;
	}

	return bullet;
}

/// <summary>
/// ����Ʈ ���� �Լ�
/// </summary>
/// <param name="bullet">���� �� �Ѿ�</param>
void InsertFBullet(FliperBullet bullet) {
	// ���ο� �Ѿ� ����
	FliperBulletNode* node = (FliperBulletNode*)malloc(sizeof(FliperBulletNode));

	// �޸� �Ҵ� ����
	if (node == NULL) {
		free(node);
		return;
	}

	// ��� ����
	node->bullet = bullet;
	node->next = NULL;

	// ���� ����Ʈ�� ��� �ִٸ�
	if (Fend == NULL) {
		Froot = node;
		Fend = node;
	}

	// ����Ʈ�� ���� �ִٸ� end�� �߰�
	else {
		Fend->next = node;
		Fend = node;
	}
	return;
}

/// <summary>
/// ����Ʈ���� ���� �Լ�
/// </summary>
/// <param name="node">���� �� ���</param>
/// <param name="pre">���� �� ����� �� ���</param>
void DeleteFBullet(FliperBulletNode* node, FliperBulletNode* pre) {
	// ����ִ� ����Ʈ�� ���
	if (Froot == NULL) {
		return;
	}

	// ��Ʈ ��带 �����ϴ� ���
	if (node == Froot) {
		Froot = node->next;
		free(node);
		return;
	}

	// ������ ����� ���
	if (node->next == NULL) {
		free(node);
		Fend = pre;
		return;
	}

	// �߰��� ���
	pre->next = node->next;
	free(node);

	return;
}

/// <summary>
/// �Ѿ��� �̵� �� ���� ���� �ߴ��� �ľ�
/// </summary>
void CheckFBullet() {
	// ��ȸ�� ���� ���
	FliperBulletNode* node = Froot;

	// ���� ����� ���� ���
	FliperBulletNode* pre = NULL;

	// ����Ʈ�� ���� ���� �� ���� ��ȸ
	for (; !(node == NULL); ) {
		node->bullet.position[1]--;

		// ��ġ ���� �� ���� ���� ��� üũ�Ͽ� ����
		if (node->bullet.position[1] == 1) {
			FliperBulletNode* temp = node->next;
			DeleteFBullet(node, pre);
			node = temp;
			continue;
		}

		// ���� ����
		pre = node;
		node = node->next;
	}
}

/// <summary>
/// �Ѿ� ����Ʈ �޸� ��ü �ݳ�
/// </summary>
void DeleteAllFBullet() {
	FliperBulletNode* node = Froot;

	for (; node != NULL;) {
		FliperBulletNode* temp = node->next;
		free(node);
		node = temp;
	}
	Froot = NULL;
	Fend = NULL;
}