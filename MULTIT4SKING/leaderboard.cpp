#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "definsize.h"
#include "leaderboard.h"

// 플레이어 정보를 담을 배열
struct Player players[100];

// 현재 총 플레이어 수
int playerCount;

// 비교 함수
int compare(const void* a, const void* b) {
	struct Player* player1 = (struct Player*)a;
	struct Player* player2 = (struct Player*)b;
	return player2->score - player1->score; // 내림차순 정렬
}

/// <summary>
/// 정렬 함수
/// </summary>
void SortPlayer() {
	// 점수순으로 정렬
	qsort(players, playerCount, sizeof(struct Player), compare);
}

/// <summary>
/// 리더보드 파일 불러오는 함수
/// </summary>
/// <returns>성공 여부</returns>
int GetLeaderBoard() {
	// 리더보드의 플레이어 숫자
	playerCount = 0;

	// 리더보드 파일 열기
	FILE* fp = NULL;
	fopen_s(&fp, "leaderboard.txt", "r");

	// 파일 열기 실패시
	if (fp == NULL) {
		return 1;
	}

	// 파일 읽기
	while (fscanf_s(fp, "%s %d", players[playerCount].name, 100, &players[playerCount].score) != EOF) {
		playerCount++;
		if (playerCount == 100) {
			break;
		}
	}

	fclose(fp);
	
	// 점수순 정렬
	SortPlayer();

	return 0;
}

/// <summary>
/// 현재 정보를 리더보드에 입력하는 함수
/// </summary>
/// <returns>성공 여부</returns>
int SetLeaderBoard() {
	// 파일 열기
	FILE* fp;
	fopen_s(&fp, "leaderboard.txt", "w");

	// 파일 열기 실패시
	if (fp == NULL) {
		return 1;
	}

	// 새로운 인풋 데이터에 대해
	SortPlayer();

	// 파일에 쓰기
	for (int i = 0; i < playerCount + 1; i++) {
		fprintf(fp, "%s %d\n", players[i].name, players[i].score); // 파일에 저장
	}

	fclose(fp);

	return 0;
}