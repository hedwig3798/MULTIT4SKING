#pragma once
#include "definsize.h"
void BoardClear(char board[][S_BOARD_SIZE_COL + 1]);
void DrawObjectAtBoard(char board[][S_BOARD_SIZE_COL + 1], char object, int x, int y, int row, int col);
void DrawBoundaryOfBoard(char board[][S_BOARD_SIZE_COL + 1]);
void InitBoard(char board[][S_BOARD_SIZE_COL + 1]);
void GameOver(int gameId);