#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "definsize.h"
#include "leaderboard.h"

// �÷��̾� ������ ���� �迭
struct Player players[100];

// ���� �� �÷��̾� ��
int playerCount;

// �� �Լ�
int compare(const void* a, const void* b) {
	struct Player* player1 = (struct Player*)a;
	struct Player* player2 = (struct Player*)b;
	return player2->score - player1->score; // �������� ����
}

/// <summary>
/// ���� �Լ�
/// </summary>
void SortPlayer() {
	// ���������� ����
	qsort(players, playerCount, sizeof(struct Player), compare);
}

/// <summary>
/// �������� ���� �ҷ����� �Լ�
/// </summary>
/// <returns>���� ����</returns>
int GetLeaderBoard() {
	// ���������� �÷��̾� ����
	playerCount = 0;

	// �������� ���� ����
	FILE* fp = NULL;
	fopen_s(&fp, "leaderboard.txt", "r");

	// ���� ���� ���н�
	if (fp == NULL) {
		return 1;
	}

	// ���� �б�
	while (fscanf_s(fp, "%s %d", players[playerCount].name, 100, &players[playerCount].score) != EOF) {
		playerCount++;
		if (playerCount == 100) {
			break;
		}
	}

	fclose(fp);
	
	// ������ ����
	SortPlayer();

	return 0;
}

/// <summary>
/// ���� ������ �������忡 �Է��ϴ� �Լ�
/// </summary>
/// <returns>���� ����</returns>
int SetLeaderBoard() {
	// ���� ����
	FILE* fp;
	fopen_s(&fp, "leaderboard.txt", "w");

	// ���� ���� ���н�
	if (fp == NULL) {
		return 1;
	}

	// ���ο� ��ǲ �����Ϳ� ����
	SortPlayer();

	// ���Ͽ� ����
	for (int i = 0; i < playerCount + 1; i++) {
		fprintf(fp, "%s %d\n", players[i].name, players[i].score); // ���Ͽ� ����
	}

	fclose(fp);

	return 0;
}