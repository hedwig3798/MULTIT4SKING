///
/// ���带 �����ϴ� �Լ� ����
/// 

#include "definsize.h"

// ���� ���� ����
extern bool gameOver[NUMBER_OF_GAME];

/// <summary>
/// ���� �ʱ�ȭ, ������ ��� ���� �������� ����
/// </summary>
/// <param name="board">����</param>
void BoardClear(char board[][S_BOARD_SIZE_COL + 1]) {
	for (int i = 0; i < S_BOARD_SIZE_ROW; i++) {
		for (int j = 0; j < S_BOARD_SIZE_COL; j++) {
			board[i][j] = ' ';
		}
	}
}

/// <summary>
///  ������Ʈ�� ���忡 �׸���
/// </summary>
/// <param name="board">�׸� ����</param>
/// <param name="object">������Ʈ ����</param>
/// <param name="x">���� ������</param>
/// <param name="y">���� ������</param>
/// <param name="row">���� ��ġ</param>
/// <param name="col">���� ��ġ</param>
void DrawObjectAtBoard(char board[][S_BOARD_SIZE_COL + 1], char object, int x, int y, int row, int col) {
	for (int i = 0; i < y; i++) {
		for (int j = 0; j < x; j++) {
			board[row + i][col + j] = object;
		}
	}
}

/// <summary>
///  ���� ��� �׸���
/// </summary>
/// <param name="board">�׸� ����</param>
void DrawBoundaryOfBoard(char board[][S_BOARD_SIZE_COL + 1]) {
	// ��� ǥ��
	for (int i = 0; i < S_BOARD_SIZE_ROW; i++) {
		board[i][0] = '#';
		board[i][S_BOARD_SIZE_COL - 1] = '#';
	}
	for (int i = 0; i < S_BOARD_SIZE_COL; i++) {
		board[0][i] = '#';
		board[S_BOARD_SIZE_ROW - 1][i] = '#';
	}
}

/// <summary>
/// ���� �ʱ�ȭ
/// ������ ������ �������� �ʱ�ȭ �ϰ� ��踦 �׸�
/// </summary>
/// <param name="board">�׸� ����</param>
void InitBoard(char board[][S_BOARD_SIZE_COL + 1]) {
	BoardClear(board);
	DrawBoundaryOfBoard(board);
}

/// <summary>
/// ���� ����
/// </summary>
/// <param name="gameId">���� ������ ���� ���̵�</param>
void GameOver(int gameId) {
	gameOver[gameId] = true;
}