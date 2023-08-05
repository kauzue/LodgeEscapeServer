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

	int msg_int = 1;
	int r_num;

	while(msg_int != -1) {

	recv(sock, &msg_int, sizeof(msg_int), 0);

	switch (msg_int) {

	case SIGNUP: {
		SignUp(sock, &players_num);
		break;
	}

	case LOGIN: {
		r_num = Login(sock, players_num);
		msg_int = -1;
		break;
	}

	}
	}

	recv(sock, &msg_int, sizeof(msg_int), 0);

	switch (msg_int) {

	case START_GAME: {

	}

	case LOAD_GAME: {
		send(sock, &s_players[r_num].s_num, NUM_MAX_SAVES_PER_PLAYER, 0);

		if (s_players[r_num].s_num == 0) {

		}

		else {
			msg_int = 0;

			for (msg_int; msg_int < s_players[r_num].s_num; ++msg_int) {
				send(sock, &s_saves[r_num][msg_int].chapter, NUM_MAX_SAVES_PER_PLAYER, 0);
				send(sock, &s_saves[r_num][msg_int].stage, NUM_MAX_SAVES_PER_PLAYER, 0);
			}

		}
	}

	case OPTION: {

	}

	}

	LeaveCriticalSection(&CriticalSection);
}

void CloseGame()
{
	DeleteCriticalSection(&CriticalSection);
}
