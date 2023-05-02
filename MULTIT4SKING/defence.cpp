#include<stdio.h>
#include<time.h>
#include<conio.h>
#include<malloc.h>
#include <windows.h>
#include "definsize.h"
#include "boardManager.h"
#include <malloc.h>
#include "soundManager.h"

/// ����ü ����
#pragma region Structes
// ����
enum Direction {
	left = 0,
	right,
	up,
	down
};

// �÷��̾ ����ؾ� �ϴ�
struct DefencePlayer {
	int position[2] = { 0, 0 };
	int sizeX = 1;
	int sizeY = 1;
	char character = '@';
};

// �÷��̾ �����ϴ�
struct DefenceShiled {
	int position[2] = { 0, 0 };
	int sizeX = 1;
	int sizeY = 1;
	char character = '#';
	int direction = up;
};

// ���ƾ� �� ������Ʈ
struct DefenceBullet {
	double position[2] = { 0, 0 };
	int sizeX = 1;
	int sizeY = 1;
	char character = '+';
	double speed = 1;
	Direction direction = left;
};

// �Ѿ� ������Ʈ ��ũ ����Ʈ
struct DefenceBulletNode {
	DefenceBullet bullet;
	DefenceBulletNode* next;
};
#pragma endregion

/// ���� ���� ����
#pragma region variables
// ���� ���� ����
extern char subGameBoard[4][S_BOARD_SIZE_ROW][S_BOARD_SIZE_COL + 1];

// ������Ʈ �Լ� ������
extern void (*updater[NUMBER_OF_GAME])(void);

// ����
extern int score;

// �÷��̾� ������Ʈ
DefencePlayer defencePlayer;
// ���� ������Ʈ
DefenceShiled defenceShiled;
// �Ѿ� ������Ʈ
DefenceBullet defenceBullet;

// ����Ʈ ��Ʈ
DefenceBulletNode* Droot;
// ����Ʈ ��
DefenceBulletNode* Dend;

// ���� �� ������Ʈ ��
int createDTerm = 0;
int oldDScore = 0;
int DTerm = 0;
#pragma endregion

/// �Լ� ����
#pragma region funcs
// �Լ� ����
void DefenceInit();
void DefenceUpdate();
DefenceBullet CreateDBullet();
void InsertDBullet(DefenceBullet bullet);
void DeleteDBullet(DefenceBulletNode* node, DefenceBulletNode* pre);
void CheckDBullet();
#pragma endregion

/// <summary>
/// �ʱ�ȭ �Լ�
/// </summary>
void DefenceInit() {
	// ���� ID
	int gameID = 2;

	// �������� �Լ� ����
	updater[gameID] = DefenceUpdate;

	// �÷��̾� �ʱ�ȭ
	defencePlayer.position[0] = S_BOARD_SIZE_ROW / 2;
	defencePlayer.position[1] = S_BOARD_SIZE_COL / 2;
	defenceShiled.sizeX = 3;
	defenceShiled.sizeY = 1;
	defenceShiled.position[0] = defencePlayer.position[0] - 1;
	defenceShiled.position[1] = defencePlayer.position[1] - (defenceShiled.sizeX / 2);

	// ����Ʈ �ʱ�ȭ
	Droot = NULL;
	Dend = NULL;

	// �� �ʱ�ȭ
	DTerm = clock();
	createDTerm = DTerm / 500;
	oldDScore = DTerm / 250;
}

/// <summary>
/// ���� ƽ ���� ����Ǵ� �Լ�
/// </summary>
void DefenceUpdate() {
	// ���� ���̵�
	int gameID = 2;

	// �� ����
	DTerm = clock();

	// Ű�ٿ� ����
	static bool keyDown[4] = {0, };

	// �÷��̾� ����
	
	if (GetAsyncKeyState(VK_W)) {
		if (!keyDown[0]) {
			keyDown[0] = true;
			EffectPlay(DEFENCE_EF);
			defenceShiled.sizeX = 3;
			defenceShiled.sizeY = 1;
			defenceShiled.position[0] = defencePlayer.position[0] - 1;
			defenceShiled.position[1] = defencePlayer.position[1] - (defenceShiled.sizeX / 2);
			defenceShiled.direction = up;
		}
	}
	else {
		keyDown[0] = false;
	}
	if (GetAsyncKeyState(VK_A)) {
		if (!keyDown[1]) {
			keyDown[1] = true;
			EffectPlay(DEFENCE_EF);
			defenceShiled.sizeX = 1;
			defenceShiled.sizeY = 3;
			defenceShiled.position[0] = defencePlayer.position[0] - (defenceShiled.sizeY / 2);
			defenceShiled.position[1] = defencePlayer.position[1] - 1;
			defenceShiled.direction = left;
		}
	}
	else {
		keyDown[1] = false;
	}
	if (GetAsyncKeyState(VK_S)) {
		if (!keyDown[2]) {
			keyDown[2] = true;
			EffectPlay(DEFENCE_EF);
			defenceShiled.sizeX = 3;
			defenceShiled.sizeY = 1;
			defenceShiled.position[0] = defencePlayer.position[0] + 1;
			defenceShiled.position[1] = defencePlayer.position[1] - (defenceShiled.sizeX / 2);
			defenceShiled.direction = down;
		}
	}
	else {
		keyDown[2] = false;
	}
	if (GetAsyncKeyState(VK_D)) {
		if (!keyDown[3]) {
			keyDown[3] = true;
			EffectPlay(DEFENCE_EF);
			defenceShiled.sizeX = 1;
			defenceShiled.sizeY = 3;
			defenceShiled.position[0] = defencePlayer.position[0] - (defenceShiled.sizeY / 2);
			defenceShiled.position[1] = defencePlayer.position[1] + 1;
			defenceShiled.direction = right;
		}
	}
	else {
		keyDown[3] = false;
	}

	// ���� �ð����� �Ѿ� ����
	if (createDTerm < DTerm / 500) {
		InsertDBullet(CreateDBullet());
		createDTerm += 7;
	}

	// ���� �ð����� �Ѿ� ������Ʈ
	if (DTerm / 250 > oldDScore) {
		oldDScore += 3;
		CheckDBullet();
	}

	// ���� �ʱ�ȭ
	InitBoard(subGameBoard[gameID]);

	// �÷��̾� ���
	DrawObjectAtBoard(subGameBoard[gameID], defencePlayer.character, defencePlayer.sizeX, defencePlayer.sizeY, defencePlayer.position[0], defencePlayer.position[1]);

	// ���� ���
	DrawObjectAtBoard(subGameBoard[gameID], defenceShiled.character, defenceShiled.sizeX, defenceShiled.sizeY, defenceShiled.position[0], defenceShiled.position[1]);

	// �Ѿ� ����Ʈ�� ��ȸ�ϸ鼭 �Ѿ� ���
	DefenceBulletNode* node = Droot;
	DefenceBulletNode* pre = NULL;
	for (; !(node == NULL); ) {
		int px = 0;
		int py = 0;
		switch (defenceShiled.direction) {
		case up:
			px = defencePlayer.position[1];
			py = defencePlayer.position[0] - 1;
			break;
		case down:
			px = defencePlayer.position[1];
			py = defencePlayer.position[0] + 1;
			break;
		case left:
			px = defencePlayer.position[1] - 1;
			py = defencePlayer.position[0];
			break;
		case right:
			px = defencePlayer.position[1] + 1;
			py = defencePlayer.position[0];
			break;
		default:
			break;
		}
		if (node->bullet.position[0] >= py && node->bullet.position[0] < py + 1 &&
			node->bullet.position[1] >= px && node->bullet.position[1] < px + 1) {
			DefenceBulletNode* temp = node->next;
			DeleteDBullet(node, pre);
			node = temp;
			continue;
		}

		DrawObjectAtBoard(subGameBoard[gameID], node->bullet.character, node->bullet.sizeX, node->bullet.sizeY, (int)node->bullet.position[0], (int)node->bullet.position[1]);
		pre = node;
		node = node->next;
	}
}

/// <summary>
/// ���� ���� �Լ�
/// </summary>
/// <returns>���� �� �Ѿ�</returns>
DefenceBullet CreateDBullet() {
	// ���� �õ� �ʱ�ȭ
	srand(time(NULL));

	// �Ѿ�
	DefenceBullet bullet;

	// ���ư� ����
	bullet.direction = (Direction)(rand() % 4);

	// ���Ῠ ���� �Ѿ� ���� ��ġ ����
	switch (bullet.direction) {
	case up:
		bullet.position[0] = 1;
		bullet.position[1] = defencePlayer.position[1];
		break;
	case down:
		bullet.position[0] = S_BOARD_SIZE_ROW - 1;
		bullet.position[1] = defencePlayer.position[1];
		break;
	case left:
		bullet.position[0] = defencePlayer.position[0];
		bullet.position[1] = 1;
		break;
	case right:
		bullet.position[0] = defencePlayer.position[0];
		bullet.position[1] = S_BOARD_SIZE_COL - 1;
		break;
	default:
		break;
	}

	// ������ �Ѿ� ��ȯ
	return bullet;
}

/// <summary>
/// ����Ʈ ���� �Լ�
/// </summary>
/// <param name="bullet">���� �� �Ѿ�</param>
void InsertDBullet(DefenceBullet bullet) {
	// ���ο� �Ѿ� ����
	DefenceBulletNode* node = (DefenceBulletNode*)malloc(sizeof(DefenceBulletNode));

	// �޸� �Ҵ� ���н�
	if (node == NULL) {
		return;
	}

	// �Ѿ� ����Ʈ ��� �ʱ�ȭ
	node->bullet = bullet;
	node->next = NULL;

	// ���� ����Ʈ�� ��� �ִٸ� ��Ʈ�� �ֱ�
	if (Dend == NULL) {
		Droot = node;
		Dend = node;
	}

	// ����Ʈ�� ���� �ִٸ� end�� �߰�
	else {
		Dend->next = node;
		Dend = node;
	}

	return;
}

/// <summary>
/// ����Ʈ���� ���� �Լ�
/// </summary>
/// <param name="node">���� �� ���</param>
/// <param name="pre">���� �� ����� �� ���</param>
void DeleteDBullet(DefenceBulletNode* node, DefenceBulletNode* pre) {
	// ����ִ� ����Ʈ�� ���
	if (Droot == NULL) {
		return;
	}

	// ��Ʈ ��带 �����ϴ� ���
	if (node == Droot) {
		Droot = node->next;
		free(node);
		return;
	}

	// ������ ��带 �����ϴ� ���
	if (node->next == NULL) {
		free(node);
		Dend = pre;
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
void CheckDBullet() {
	// ��ȸ�� ���� ���
	DefenceBulletNode* node = Droot;
	// ���� ����� ���� ���
	DefenceBulletNode* pre = NULL;

	// ����Ʈ�� ���� ���� �� ���� ��ȸ
	for (; !(node == NULL); ) {
		// ����� ���⿡ ���� ��ġ ����
		switch (node->bullet.direction) {
		case up:
			node->bullet.position[0] += node->bullet.speed;
			break;
		case down:
			node->bullet.position[0] -= node->bullet.speed;
			break;
		case left:
			node->bullet.position[1] += node->bullet.speed;
			break;
		case right:
			node->bullet.position[1] -= node->bullet.speed;
			break;
		default:
			break;
		}

		// ��ġ ���� �� ���� ���� ��� üũ�Ͽ� ����
		if ((node->bullet.position[0] >= defencePlayer.position[0] && node->bullet.position[0] < defencePlayer.position[0] + 1) &&
			(node->bullet.position[1] >= defencePlayer.position[1] && node->bullet.position[1] < defencePlayer.position[1] + 1)) {
			DefenceBulletNode* temp = node->next;
			DeleteDBullet(node, pre);
			node = temp;
			GameOver(2);
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
void DeleteAllDBullet() {
	DefenceBulletNode* node = Droot;

	for(;node != NULL;) {
		DefenceBulletNode* temp = node->next;
		free(node);
		node = temp;
	}
	Droot = NULL;
	Dend = NULL;
}