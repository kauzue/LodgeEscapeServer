#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <winsock2.h>

#include "game.h"
#include "login.h"

player_t s_players[NUM_MAX_PLAYERS];
save_t s_saves[NUM_MAX_PLAYERS][NUM_MAX_SAVES_PER_PLAYER];

int players_num;
CRITICAL_SECTION CriticalSection;

void InitGame()
{
	InitializeCriticalSectionAndSpinCount(&CriticalSection, 2000);

	FILE* pb = fopen("player.bin", "rb");

	if (pb == NULL) {
		puts("파일 오픈 실패");
		return;
	}

	players_num = fread(s_players, sizeof(player_t), NUM_MAX_PLAYERS, pb);
}

int Game(SOCKET sock)
{
	EnterCriticalSection(&CriticalSection);

	int msg_int;

	recv(sock, &msg_int, sizeof(msg_int), 0);

	switch (msg_int) {
		
	case SIGNUP: {
		SignUp(sock, players_num);
		break;
	}

	case LOGIN: {
		printf("로그인 \n");
		break;
	}

	}

	LeaveCriticalSection(&CriticalSection);
}

void CloseGame()
{
	DeleteCriticalSection(&CriticalSection);
}