#include<stdio.h>
#include<time.h>
#include<conio.h>
#include<malloc.h>
#include <windows.h>
#include "definsize.h"
#include "boardManager.h"
#include <malloc.h>
#include "soundManager.h"

/// 구조체 모음
#pragma region Structes
// 방향
enum Direction {
	left = 0,
	right,
	up,
	down
};

// 플레이어가 방어해야 하는
struct DefencePlayer {
	int position[2] = { 0, 0 };
	int sizeX = 1;
	int sizeY = 1;
	char character = '@';
};

// 플레이어가 조작하는
struct DefenceShiled {
	int position[2] = { 0, 0 };
	int sizeX = 1;
	int sizeY = 1;
	char character = '#';
	int direction = up;
};

// 막아야 될 오브젝트
struct DefenceBullet {
	double position[2] = { 0, 0 };
	int sizeX = 1;
	int sizeY = 1;
	char character = '+';
	double speed = 1;
	Direction direction = left;
};

// 총알 오브젝트 링크 리스트
struct DefenceBulletNode {
	DefenceBullet bullet;
	DefenceBulletNode* next;
};
#pragma endregion

/// 전역 변수 모음
#pragma region variables
// 서브 게임 보드
extern char subGameBoard[4][S_BOARD_SIZE_ROW][S_BOARD_SIZE_COL + 1];

// 업데이트 함수 포인터
extern void (*updater[NUMBER_OF_GAME])(void);

// 점수
extern int score;

// 플레이어 오브젝트
DefencePlayer defencePlayer;
// 방패 오브젝트
DefenceShiled defenceShiled;
// 총알 오브젝트
DefenceBullet defenceBullet;

// 리스트 루트
DefenceBulletNode* Droot;
// 리스트 끝
DefenceBulletNode* Dend;

// 생성 및 업데이트 텀
int createDTerm = 0;
int oldDScore = 0;
int DTerm = 0;
#pragma endregion

/// 함수 선언
#pragma region funcs
// 함수 선언
void DefenceInit();
void DefenceUpdate();
DefenceBullet CreateDBullet();
void InsertDBullet(DefenceBullet bullet);
void DeleteDBullet(DefenceBulletNode* node, DefenceBulletNode* pre);
void CheckDBullet();
#pragma endregion

/// <summary>
/// 초기화 함수
/// </summary>
void DefenceInit() {
	// 게임 ID
	int gameID = 2;

	// 업데이터 함수 전달
	updater[gameID] = DefenceUpdate;

	// 플레이어 초기화
	defencePlayer.position[0] = S_BOARD_SIZE_ROW / 2;
	defencePlayer.position[1] = S_BOARD_SIZE_COL / 2;
	defenceShiled.sizeX = 3;
	defenceShiled.sizeY = 1;
	defenceShiled.position[0] = defencePlayer.position[0] - 1;
	defenceShiled.position[1] = defencePlayer.position[1] - (defenceShiled.sizeX / 2);

	// 리스트 초기화
	Droot = NULL;
	Dend = NULL;

	// 텀 초기화
	DTerm = clock();
	createDTerm = DTerm / 500;
	oldDScore = DTerm / 250;
}

/// <summary>
/// 게임 틱 마다 실행되는 함수
/// </summary>
void DefenceUpdate() {
	// 게임 아이디
	int gameID = 2;

	// 텀 측정
	DTerm = clock();

	// 키다운 감지
	static bool keyDown[4] = {0, };

	// 플레이어 조작
	
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

	// 일정 시간마다 총알 생성
	if (createDTerm < DTerm / 500) {
		InsertDBullet(CreateDBullet());
		createDTerm += 7;
	}

	// 일정 시간마다 총알 업데이트
	if (DTerm / 250 > oldDScore) {
		oldDScore += 3;
		CheckDBullet();
	}

	// 보드 초기화
	InitBoard(subGameBoard[gameID]);

	// 플레이어 출력
	DrawObjectAtBoard(subGameBoard[gameID], defencePlayer.character, defencePlayer.sizeX, defencePlayer.sizeY, defencePlayer.position[0], defencePlayer.position[1]);

	// 방패 출력
	DrawObjectAtBoard(subGameBoard[gameID], defenceShiled.character, defenceShiled.sizeX, defenceShiled.sizeY, defenceShiled.position[0], defenceShiled.position[1]);

	// 총알 리스트를 순회하면서 총알 출력
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
/// 총할 생성 함수
/// </summary>
/// <returns>생성 된 총알</returns>
DefenceBullet CreateDBullet() {
	// 랜덤 시드 초기화
	srand(time(NULL));

	// 총알
	DefenceBullet bullet;

	// 나아갈 방향
	bullet.direction = (Direction)(rand() % 4);

	// 방햐엥 따른 총알 생성 위치 지정
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

	// 생성된 총알 반환
	return bullet;
}

/// <summary>
/// 리스트 삽입 함수
/// </summary>
/// <param name="bullet">삽입 될 총알</param>
void InsertDBullet(DefenceBullet bullet) {
	// 새로운 총알 생성
	DefenceBulletNode* node = (DefenceBulletNode*)malloc(sizeof(DefenceBulletNode));

	// 메모리 할당 실패시
	if (node == NULL) {
		return;
	}

	// 총알 리스트 노드 초기화
	node->bullet = bullet;
	node->next = NULL;

	// 만일 리스트가 비어 있다면 루트에 넣기
	if (Dend == NULL) {
		Droot = node;
		Dend = node;
	}

	// 리스트에 무언가 있다면 end에 추가
	else {
		Dend->next = node;
		Dend = node;
	}

	return;
}

/// <summary>
/// 리스트에서 제거 함수
/// </summary>
/// <param name="node">제거 될 노드</param>
/// <param name="pre">제거 될 노드의 전 노드</param>
void DeleteDBullet(DefenceBulletNode* node, DefenceBulletNode* pre) {
	// 비어있는 리스트인 경우
	if (Droot == NULL) {
		return;
	}

	// 루트 노드를 제거하는 경우
	if (node == Droot) {
		Droot = node->next;
		free(node);
		return;
	}

	// 마지막 노드를 제거하는 경우
	if (node->next == NULL) {
		free(node);
		Dend = pre;
		return;
	}

	// 중간인 경우
	pre->next = node->next;
	free(node);

	return;
}

/// <summary>
/// 총알의 이동 및 끝에 도달 했는지 파악
/// </summary>
void CheckDBullet() {
	// 순회를 위한 노드
	DefenceBulletNode* node = Droot;
	// 현재 노드의 이전 노드
	DefenceBulletNode* pre = NULL;

	// 리스트의 끝에 닿을 때 까지 순회
	for (; !(node == NULL); ) {
		// 노드의 방향에 따라 위치 증감
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

		// 위치 증감 후 끝에 닿은 경우 체크하여 제거
		if ((node->bullet.position[0] >= defencePlayer.position[0] && node->bullet.position[0] < defencePlayer.position[0] + 1) &&
			(node->bullet.position[1] >= defencePlayer.position[1] && node->bullet.position[1] < defencePlayer.position[1] + 1)) {
			DefenceBulletNode* temp = node->next;
			DeleteDBullet(node, pre);
			node = temp;
			GameOver(2);
			continue;
		}

		// 다음 노드로
		pre = node;
		node = node->next;
	}
}

/// <summary>
/// 총알 리스트 메모리 전체 반납
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