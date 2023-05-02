#include<stdio.h>
#include<time.h>
#include<malloc.h>
#include <windows.h>
#include "definsize.h"
#include "boardManager.h"
#include "soundManager.h"

/// 구조체 모음
#pragma region Structes
// 플레이어 오브젝트
struct DodgePlayer {
	int position[2] = { 0, 0 };
	int sizeX = 1;
	int sizeY = 1;
	char character = '@';
	int speed = 1;
};

// 괄호 오브젝트
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

// 피해야 될 오브젝트
struct DodgeBullet {
	double position[2] = { 0, 0 };
	int sizeX = 1;
	int sizeY = 1;
	char character[2] = { '<', '>' };
	int direction = 0;
	double speed = 1;
};

// 총알 오브젝트 링크 리스트
struct DodgeBulletNode {
	DodgeBullet bullet;
	DodgeBulletNode* next;
};
#pragma endregion

/// 전역 변수 모음
#pragma region variables
// 서브 게임 보드
extern char subGameBoard[4][S_BOARD_SIZE_ROW][S_BOARD_SIZE_COL + 1];

// 업데이터 함수를 전달할 변수
extern void (*updater[NUMBER_OF_GAME])(void);

// 점수 변수
extern int score;

// 플레이어 오브젝트
DodgePlayer dodgePlayer;

// 플레이어를 감싸는 괄호 오브젝트
DodgeOpenCase dodgeOpenCase;
DodgeCloseCase dodgeCloseCase;

// 리스트 시작과 끝
DodgeBulletNode* root;
DodgeBulletNode* end;

// 키다운을 체크하는 변수
bool keyup = false;
bool keydown = false;

// 생성 및 업데이트 텀 변수
int oldScore = 0;
int term = 0;
int createTerm = 0;
#pragma endregion

/// 함수 선언
#pragma region funcs
// 함수 선언
void DodgeInit();
void DodgeUpdate();
DodgeBullet CreateBullet();
void InsertBullet(DodgeBullet bullet);
void DeleteBullet(DodgeBulletNode* node, DodgeBulletNode* pre);
void CheckBullet();
#pragma endregion

/// <summary>
/// 초기화 함수
/// </summary>
void DodgeInit() {
	// 게임 ID
	int gameID = 1;

	// 업데이터 함수 전달
	updater[gameID] = DodgeUpdate;

	// 보드 초기화
	InitBoard(subGameBoard[gameID]);

	// 플레이어 위치 초기화
	dodgePlayer.position[0] = S_BOARD_SIZE_ROW / 2;
	dodgePlayer.position[1] = S_BOARD_SIZE_COL / 2;

	// 발판 오브젝트 초기화
	dodgeOpenCase.position[0] = S_BOARD_SIZE_ROW / 2 - dodgeOpenCase.sizeY / 2;
	dodgeOpenCase.position[1] = S_BOARD_SIZE_COL / 2 - dodgePlayer.sizeX;
	dodgeCloseCase.position[0] = S_BOARD_SIZE_ROW / 2 - dodgeCloseCase.sizeY / 2;
	dodgeCloseCase.position[1] = S_BOARD_SIZE_COL / 2 + dodgePlayer.sizeX;;

	// 링크 리스트 초기화
	root = NULL;
	end = NULL;

	// 텀 초기화
	term = clock();
	oldScore = term / 250;
	createTerm = term / 500;
}

/// <summary>
/// 업데이트 함수
/// </summary>
void DodgeUpdate() {
	// 게임 ID
	int gameID = 1;

	// 텀 시작
	term = clock();

	// 키입력 처리
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

	// 총알 생성
	if (createTerm < term / 500) {
		InsertBullet(CreateBullet());
		createTerm += 7;
	}

	// 총알 위치 업데이트
	if (term / 250 > oldScore) {
		oldScore += 3;
		CheckBullet();
	}

	// 보드 초기화
	InitBoard(subGameBoard[gameID]);

	// 플레이어 출력
	DrawObjectAtBoard(subGameBoard[gameID], dodgePlayer.character, dodgePlayer.sizeX, dodgePlayer.sizeY, dodgePlayer.position[0], dodgePlayer.position[1]);
	
	// 괄호 출력
	DrawObjectAtBoard(subGameBoard[gameID], dodgeOpenCase.character, dodgeOpenCase.sizeX, dodgeOpenCase.sizeY, dodgeOpenCase.position[0], dodgeOpenCase.position[1]);
	DrawObjectAtBoard(subGameBoard[gameID], dodgeCloseCase.character, dodgeCloseCase.sizeX, dodgeCloseCase.sizeY, dodgeCloseCase.position[0], dodgeCloseCase.position[1]);
	
	// 총알 출력
	DodgeBulletNode* node = root;
	for (; !(node == NULL); node = node->next) {
		DrawObjectAtBoard(subGameBoard[gameID], node->bullet.character[node->bullet.direction], node->bullet.sizeX, node->bullet.sizeY, (int)node->bullet.position[0], (int)node->bullet.position[1]);
	}
}

/// <summary>
/// 총할 생성 함수
/// </summary>
/// <returns>생성 된 총알</returns>
DodgeBullet CreateBullet() {
	// 랜덤 시드 초기화
	srand(time(NULL));

	// 총알 생성
	DodgeBullet bullet;

	// 나아갈 방향
	bullet.direction = rand() % 2;

	// 랜덤 Y위치
	bullet.position[0] = dodgeOpenCase.position[0] + (rand() % 5);

	// 방향에 따른 X 위치
	if (!bullet.direction) {
		bullet.position[1] = S_BOARD_SIZE_COL - 2;
	}
	else {
		bullet.position[1] = 1;
	}

	return bullet;
}

/// <summary>
/// 리스트 삽입 함수
/// </summary>
/// <param name="bullet">삽입 될 총알</param>
void InsertBullet(DodgeBullet bullet) {
	// 새로운 총알 생성
	DodgeBulletNode* node = (DodgeBulletNode*)malloc(sizeof(DodgeBulletNode));

	// 메모리 할당 실패
	if (node == NULL) {
		return;
	}

	// 노드 생성
	node->bullet = bullet;
	node->next = NULL;

	// 만일 리스트가 비어 있다면
	if (end == NULL) {
		root = node;
		end = node;
	}
	// 리스트에 무언가 있다면 end에 추가
	else {
		end->next = node;
		end = node;
	}

	return;
}

/// <summary>
/// 리스트에서 제거 함수
/// </summary>
/// <param name="node">제거 될 노드</param>
/// <param name="pre">제거 될 노드의 전 노드</param>
void DeleteBullet(DodgeBulletNode* node, DodgeBulletNode* pre) {
	// 비어있는 리스트인 경우
	if (root == NULL) {
		return;
	}

	// 루트 노드를 제거하는 경우
	if (node == root) {
		root = node->next;
		free(node);
		return;
	}

	// 마지막 노드인 경우
	if (node->next == NULL) {
		free(node);
		end = pre;
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
void CheckBullet() {
	// 순회를 위한 노드
	DodgeBulletNode* node = root;
	// 현재 노드의 이전 노드
	DodgeBulletNode* pre = NULL;

	// 리스트의 끝에 닿을 때 까지 순회
	for (; !(node == NULL); ) {
		// 노드의 방향에 따라 위치 증감
		if (node->bullet.direction == 1) {
			node->bullet.position[1] += node->bullet.speed;
		}
		else {
			node->bullet.position[1] -= node->bullet.speed;
		}

		// 위치 증감 후 끝에 닿은 경우 체크하여 제거
		if (node->bullet.position[1] <= 0 || node->bullet.position[1] >= S_BOARD_SIZE_COL - 1) {
			DodgeBulletNode* temp = node->next;
			DeleteBullet(node, pre);
			node = temp;
			continue;
		}

		// 플레이어게 닿은 경우 페크하여 게임 오버
		if (node->bullet.position[0] >= dodgePlayer.position[0] && node->bullet.position[0] < dodgePlayer.position[0] + 1 &&
			node->bullet.position[1] >= dodgePlayer.position[1] && node->bullet.position[1] < dodgePlayer.position[1] + 1) {
			GameOver(1);
		}

		// 다음 노드로
		pre = node;
		node = node->next;
	}
}

/// <summary>
/// 총알 리스트 메모리 전체 반납
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
