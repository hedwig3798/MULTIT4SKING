///
/// 보드를 관리하는 함수 모음
/// 

#include "definsize.h"

// 게임 오버 변수
extern bool gameOver[NUMBER_OF_GAME];

/// <summary>
/// 보드 초기화, 보드의 모든 값을 공백으로 설정
/// </summary>
/// <param name="board">보드</param>
void BoardClear(char board[][S_BOARD_SIZE_COL + 1]) {
	for (int i = 0; i < S_BOARD_SIZE_ROW; i++) {
		for (int j = 0; j < S_BOARD_SIZE_COL; j++) {
			board[i][j] = ' ';
		}
	}
}

/// <summary>
///  오브젝트를 보드에 그리기
/// </summary>
/// <param name="board">그릴 보드</param>
/// <param name="object">오브젝트 문자</param>
/// <param name="x">가로 사이즈</param>
/// <param name="y">세로 사이즈</param>
/// <param name="row">가로 위치</param>
/// <param name="col">세로 위치</param>
void DrawObjectAtBoard(char board[][S_BOARD_SIZE_COL + 1], char object, int x, int y, int row, int col) {
	for (int i = 0; i < y; i++) {
		for (int j = 0; j < x; j++) {
			board[row + i][col + j] = object;
		}
	}
}

/// <summary>
///  보드 경계 그리기
/// </summary>
/// <param name="board">그릴 보드</param>
void DrawBoundaryOfBoard(char board[][S_BOARD_SIZE_COL + 1]) {
	// 경계 표시
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
/// 보드 초기화
/// 보드의 내용을 공백으로 초기화 하고 경계를 그림
/// </summary>
/// <param name="board">그릴 보드</param>
void InitBoard(char board[][S_BOARD_SIZE_COL + 1]) {
	BoardClear(board);
	DrawBoundaryOfBoard(board);
}

/// <summary>
/// 게임 오버
/// </summary>
/// <param name="gameId">게임 오버된 게임 아이디</param>
void GameOver(int gameId) {
	gameOver[gameId] = true;
}