#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "definsize.h"
struct Player {
	char name[4];
	int score;
};

extern struct Player players[100];

extern int playerCount;

int GetLeaderBoard();

int SetLeaderBoard();