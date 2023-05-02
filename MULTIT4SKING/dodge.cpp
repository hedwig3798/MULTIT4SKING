#include<stdio.h>
#include<time.h>
#include<malloc.h>
#include <windows.h>
#include "definsize.h"
#include "boardManager.h"
#include "soundManager.h"

/// ����ü ����
#pragma region Structes
// �÷��̾� ������Ʈ
struct DodgePlayer {
	int position[2] = { 0, 0 };
	int sizeX = 1;
	int sizeY = 1;
	char character = '@';
	int speed = 1;
};

// ��ȣ ������Ʈ
struct DodgeOpenCase {
	int position[2] = { 0, 0 };
	int sizeX = 1;
	int sizeY = 5;
	char character = '[';
};
struct DodgeCloseCase {
	int position[2] = { 0, 0 };
	int sizeX = 1;
	int sizeY = 5;
	char character = ']';
};

// ���ؾ� �� ������Ʈ
struct DodgeBullet {
	double position[2] = { 0, 0 };
	int sizeX = 1;
	int sizeY = 1;
	char character[2] = { '<', '>' };
	int direction = 0;
	double speed = 1;
};

// �Ѿ� ������Ʈ ��ũ ����Ʈ
struct DodgeBulletNode {
	DodgeBullet bullet;
	DodgeBulletNode* next;
};
#pragma endregion

/// ���� ���� ����
#pragma region variables
// ���� ���� ����
extern char subGameBoard[4][S_BOARD_SIZE_ROW][S_BOARD_SIZE_COL + 1];

// �������� �Լ��� ������ ����
extern void (*updater[NUMBER_OF_GAME])(void);

// ���� ����
extern int score;

// �÷��̾� ������Ʈ
DodgePlayer dodgePlayer;

// �÷��̾ ���δ� ��ȣ ������Ʈ
DodgeOpenCase dodgeOpenCase;
DodgeCloseCase dodgeCloseCase;

// ����Ʈ ���۰� ��
DodgeBulletNode* root;
DodgeBulletNode* end;

// Ű�ٿ��� üũ�ϴ� ����
bool keyup = false;
bool keydown = false;

// ���� �� ������Ʈ �� ����
int oldScore = 0;
int term = 0;
int createTerm = 0;
#pragma endregion

/// �Լ� ����
#pragma region funcs
// �Լ� ����
void DodgeInit();
void DodgeUpdate();
DodgeBullet CreateBullet();
void InsertBullet(DodgeBullet bullet);
void DeleteBullet(DodgeBulletNode* node, DodgeBulletNode* pre);
void CheckBullet();
#pragma endregion

/// <summary>
/// �ʱ�ȭ �Լ�
/// </summary>
void DodgeInit() {
	// ���� ID
	int gameID = 1;

	// �������� �Լ� ����
	updater[gameID] = DodgeUpdate;

	// ���� �ʱ�ȭ
	InitBoard(subGameBoard[gameID]);

	// �÷��̾� ��ġ �ʱ�ȭ
	dodgePlayer.position[0] = S_BOARD_SIZE_ROW / 2;
	dodgePlayer.position[1] = S_BOARD_SIZE_COL / 2;

	// ���� ������Ʈ �ʱ�ȭ
	dodgeOpenCase.position[0] = S_BOARD_SIZE_ROW / 2 - dodgeOpenCase.sizeY / 2;
	dodgeOpenCase.position[1] = S_BOARD_SIZE_COL / 2 - dodgePlayer.sizeX;
	dodgeCloseCase.position[0] = S_BOARD_SIZE_ROW / 2 - dodgeCloseCase.sizeY / 2;
	dodgeCloseCase.position[1] = S_BOARD_SIZE_COL / 2 + dodgePlayer.sizeX;;

	// ��ũ ����Ʈ �ʱ�ȭ
	root = NULL;
	end = NULL;

	// �� �ʱ�ȭ
	term = clock();
	oldScore = term / 250;
	createTerm = term / 500;
}

/// <summary>
/// ������Ʈ �Լ�
/// </summary>
void DodgeUpdate() {
	// ���� ID
	int gameID = 1;

	// �� ����
	term = clock();

	// Ű�Է� ó��
	if (GetAsyncKeyState(VK_UP) & 0x8000) {
		if (!keyup) {
			EffectPlay(DODGE_EF);
			keyup = true;
			if (dodgePlayer.position[0] > dodgeOpenCase.position[0] && GetAsyncKeyState(VK_UP)) {
				dodgePlayer.position[0]--;
			}
		}
	}
	else {
		keyup = false;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		if (!keydown) {
			EffectPlay(DODGE_EF);
			keydown = true;
			if (dodgePlayer.position[0] < dodgeOpenCase.position[0] + dodgeOpenCase.sizeY - 1) {
				dodgePlayer.position[0]++;
			}
		}
	}
	else {
		keydown = false;
	}

	// �Ѿ� ����
	if (createTerm < term / 500) {
		InsertBullet(CreateBullet());
		createTerm += 7;
	}

	// �Ѿ� ��ġ ������Ʈ
	if (term / 250 > oldScore) {
		oldScore += 3;
		CheckBullet();
	}

	// ���� �ʱ�ȭ
	InitBoard(subGameBoard[gameID]);

	// �÷��̾� ���
	DrawObjectAtBoard(subGameBoard[gameID], dodgePlayer.character, dodgePlayer.sizeX, dodgePlayer.sizeY, dodgePlayer.position[0], dodgePlayer.position[1]);
	
	// ��ȣ ���
	DrawObjectAtBoard(subGameBoard[gameID], dodgeOpenCase.character, dodgeOpenCase.sizeX, dodgeOpenCase.sizeY, dodgeOpenCase.position[0], dodgeOpenCase.position[1]);
	DrawObjectAtBoard(subGameBoard[gameID], dodgeCloseCase.character, dodgeCloseCase.sizeX, dodgeCloseCase.sizeY, dodgeCloseCase.position[0], dodgeCloseCase.position[1]);
	
	// �Ѿ� ���
	DodgeBulletNode* node = root;
	for (; !(node == NULL); node = node->next) {
		DrawObjectAtBoard(subGameBoard[gameID], node->bullet.character[node->bullet.direction], node->bullet.sizeX, node->bullet.sizeY, (int)node->bullet.position[0], (int)node->bullet.position[1]);
	}
}

/// <summary>
/// ���� ���� �Լ�
/// </summary>
/// <returns>���� �� �Ѿ�</returns>
DodgeBullet CreateBullet() {
	// ���� �õ� �ʱ�ȭ
	srand(time(NULL));

	// �Ѿ� ����
	DodgeBullet bullet;

	// ���ư� ����
	bullet.direction = rand() % 2;

	// ���� Y��ġ
	bullet.position[0] = dodgeOpenCase.position[0] + (rand() % 5);

	// ���⿡ ���� X ��ġ
	if (!bullet.direction) {
		bullet.position[1] = S_BOARD_SIZE_COL - 2;
	}
	else {
		bullet.position[1] = 1;
	}

	return bullet;
}

/// <summary>
/// ����Ʈ ���� �Լ�
/// </summary>
/// <param name="bullet">���� �� �Ѿ�</param>
void InsertBullet(DodgeBullet bullet) {
	// ���ο� �Ѿ� ����
	DodgeBulletNode* node = (DodgeBulletNode*)malloc(sizeof(DodgeBulletNode));

	// �޸� �Ҵ� ����
	if (node == NULL) {
		return;
	}

	// ��� ����
	node->bullet = bullet;
	node->next = NULL;

	// ���� ����Ʈ�� ��� �ִٸ�
	if (end == NULL) {
		root = node;
		end = node;
	}
	// ����Ʈ�� ���� �ִٸ� end�� �߰�
	else {
		end->next = node;
		end = node;
	}

	return;
}

/// <summary>
/// ����Ʈ���� ���� �Լ�
/// </summary>
/// <param name="node">���� �� ���</param>
/// <param name="pre">���� �� ����� �� ���</param>
void DeleteBullet(DodgeBulletNode* node, DodgeBulletNode* pre) {
	// ����ִ� ����Ʈ�� ���
	if (root == NULL) {
		return;
	}

	// ��Ʈ ��带 �����ϴ� ���
	if (node == root) {
		root = node->next;
		free(node);
		return;
	}

	// ������ ����� ���
	if (node->next == NULL) {
		free(node);
		end = pre;
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
void CheckBullet() {
	// ��ȸ�� ���� ���
	DodgeBulletNode* node = root;
	// ���� ����� ���� ���
	DodgeBulletNode* pre = NULL;

	// ����Ʈ�� ���� ���� �� ���� ��ȸ
	for (; !(node == NULL); ) {
		// ����� ���⿡ ���� ��ġ ����
		if (node->bullet.direction == 1) {
			node->bullet.position[1] += node->bullet.speed;
		}
		else {
			node->bullet.position[1] -= node->bullet.speed;
		}

		// ��ġ ���� �� ���� ���� ��� üũ�Ͽ� ����
		if (node->bullet.position[1] <= 0 || node->bullet.position[1] >= S_BOARD_SIZE_COL - 1) {
			DodgeBulletNode* temp = node->next;
			DeleteBullet(node, pre);
			node = temp;
			continue;
		}

		// �÷��̾�� ���� ��� ��ũ�Ͽ� ���� ����
		if (node->bullet.position[0] >= dodgePlayer.position[0] && node->bullet.position[0] < dodgePlayer.position[0] + 1 &&
			node->bullet.position[1] >= dodgePlayer.position[1] && node->bullet.position[1] < dodgePlayer.position[1] + 1) {
			GameOver(1);
		}

		// ���� ����
		pre = node;
		node = node->next;
	}
}

/// <summary>
/// �Ѿ� ����Ʈ �޸� ��ü �ݳ�
/// </summary>
void DeleteAllBullet() {
	DodgeBulletNode* node = root;

	for (; node != NULL;) {
		DodgeBulletNode* temp = node->next;
		free(node);
		node = temp;
	}

	root = NULL;
	end = NULL;
}
