#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#include "game.h"

player_t s_players[NUM_MAX_PLAYERS];
save_t s_saves[NUM_MAX_PLAYERS][NUM_MAX_SAVES_PER_PLAYER];

int InitGame()
{
	int players_num;

	FILE* pb = fopen("player.bin", "rb");

	if (pb == NULL) {
		puts("파일 오픈 실패");
		return;
	}

	return players_num = fread(s_players, sizeof(player_t), NUM_MAX_PLAYERS, pb);
}

void Game(SOCKET sock, CRITICAL_SECTION Cs)
{
	EnterCriticalSection(&Cs);
	LeaveCriticalSection(&Cs);
}