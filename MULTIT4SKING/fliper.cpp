#include<stdio.h>
#include<time.h>
#include<malloc.h>
#include <windows.h>
#include "definsize.h"
#include "boardManager.h"
#include "soundManager.h"

/// 구조체 모음
#pragma region structes
// 플레이어 오브젝트
struct FliperPlayer {
	int position[2] = { 0, 0 };
	int sizeX = 1;
	int sizeY = 3;
	char character = '@';
	int speed = 1;
	bool isUp = false;
};

// 기준선 오브젝트
struct FliperHorizon {
	int position[2] = { 0, 0 };
	int sizeX = S_BOARD_SIZE_COL - 2;
	int sizeY = 1;
	char character = '-';
	int speed = 1;
};

// 피해야 될 오브젝트
struct FliperBullet {
	int position[2] = { 0, 0 };
	int sizeX = 1;
	int sizeY = 3;
	char character[2] = { 'W', 'M' };
	bool direction = 0;
};

// 총알 오브젝트 링크 리스트
struct FliperBulletNode {
	FliperBullet bullet;
	FliperBulletNode* next;
};
#pragma endregion

/// 전역 변수 모음
#pragma region variables
// 서브 게임 보드
extern char subGameBoard[4][S_BOARD_SIZE_ROW][S_BOARD_SIZE_COL + 1];

// 업데이터 함수 전달 포인터
extern void (*updater[NUMBER_OF_GAME])(void);

// 플레이어 오브젝트
FliperPlayer fliperPlayer;

// 수평선 오브젝트
FliperHorizon fliperHorizon;

// 총알 오브젝트
FliperBullet fliperBullet;

// 리스트 루트 및 끝
FliperBulletNode* Froot;
FliperBulletNode* Fend;

// 키다운 체크 변수
bool keyDown = false;

// 텀 변수
int createFTerm = 0;
int oldFScore = 0;
int FTerm = 0;
#pragma endregion

/// 함수 선언 모음
#pragma region funcs
void FliperInit();
void FliperUpdate();
FliperBullet CreateFBullet();
void InsertFBullet(FliperBullet bullet);
void DeleteFBullet(FliperBulletNode* node, FliperBulletNode* pre);
void CheckFBullet();
#pragma endregion

/// <summary>
/// 초기화 함수
/// </summary>
void FliperInit() {
	// 게임 ID
	int gameID = 3;

	// 업데이터 함수 전달
	updater[gameID] = FliperUpdate;

	// 보드 초기화
	InitBoard(subGameBoard[gameID]);

	// 플레이어 초기화
	fliperPlayer.position[0] = S_BOARD_SIZE_ROW / 2 + 1;
	fliperPlayer.position[1] = 3;
	fliperPlayer.isUp = false;

	// 수평선 초기화
	fliperHorizon.position[0] = S_BOARD_SIZE_ROW / 2;
	fliperHorizon.position[1] = 1;

	// 총알 리스트 초기화
	Froot = NULL;
	Fend = NULL;

	// 텀 초기화
	FTerm = clock();
	createFTerm = FTerm / 500;
	oldFScore = FTerm / 250;
}

/// <summary>
/// 게임 틱 마다 실행되는 함수
/// </summary>
void FliperUpdate() {
	// 게임 아이디
	int gameID = 3;

	// 텀 시작
	FTerm = clock();

	// 키입력 처리
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

	// 생성
	if (createFTerm < FTerm / 500) {
		InsertFBullet(CreateFBullet());
		createFTerm += 7;
	}

	// 이동
	if (FTerm / 250 > oldFScore) {
		oldFScore += 3;
		CheckFBullet();
	}

	// 보드 초기화
	InitBoard(subGameBoard[gameID]);

	// 플레이어 출력
	DrawObjectAtBoard(subGameBoard[gameID], fliperPlayer.character, fliperPlayer.sizeX, fliperPlayer.sizeY, fliperPlayer.position[0], fliperPlayer.position[1]);
	
	// 수평선 출력
	DrawObjectAtBoard(subGameBoard[gameID], fliperHorizon.character, fliperHorizon.sizeX, fliperHorizon.sizeY, fliperHorizon.position[0], fliperHorizon.position[1]);
	
	// 총알 출력
	FliperBulletNode* node = Froot;
	FliperBulletNode* pre = NULL;
	// 만일 플레이어와 충돌 시 제거 및 게임 오버
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
/// 총알 생성 함수
/// </summary>
/// <returns>생성 된 총알</returns>
FliperBullet CreateFBullet() {
	// 랜덤 시드 초기화
	srand(time(NULL));

	// 총알 생성
	FliperBullet bullet;

	// 방향 설정
	bullet.direction = rand() % 2;

	// 방향에 따른 위치 지정
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
/// 리스트 삽입 함수
/// </summary>
/// <param name="bullet">삽입 될 총알</param>
void InsertFBullet(FliperBullet bullet) {
	// 새로운 총알 생성
	FliperBulletNode* node = (FliperBulletNode*)malloc(sizeof(FliperBulletNode));

	// 메모리 할당 실패
	if (node == NULL) {
		free(node);
		return;
	}

	// 노드 생성
	node->bullet = bullet;
	node->next = NULL;

	// 만일 리스트가 비어 있다면
	if (Fend == NULL) {
		Froot = node;
		Fend = node;
	}

	// 리스트에 무언가 있다면 end에 추가
	else {
		Fend->next = node;
		Fend = node;
	}
	return;
}

/// <summary>
/// 리스트에서 제거 함수
/// </summary>
/// <param name="node">제거 될 노드</param>
/// <param name="pre">제거 될 노드의 전 노드</param>
void DeleteFBullet(FliperBulletNode* node, FliperBulletNode* pre) {
	// 비어있는 리스트인 경우
	if (Froot == NULL) {
		return;
	}

	// 루트 노드를 제거하는 경우
	if (node == Froot) {
		Froot = node->next;
		free(node);
		return;
	}

	// 마지막 노드인 경우
	if (node->next == NULL) {
		free(node);
		Fend = pre;
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
void CheckFBullet() {
	// 순회를 위한 노드
	FliperBulletNode* node = Froot;

	// 현재 노드의 이전 노드
	FliperBulletNode* pre = NULL;

	// 리스트의 끝에 닿을 때 까지 순회
	for (; !(node == NULL); ) {
		node->bullet.position[1]--;

		// 위치 증감 후 끝에 닿은 경우 체크하여 제거
		if (node->bullet.position[1] == 1) {
			FliperBulletNode* temp = node->next;
			DeleteFBullet(node, pre);
			node = temp;
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