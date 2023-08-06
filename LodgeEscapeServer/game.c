#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <winsock2.h>
#include <stdbool.h>

#include "game.h"
#include "login.h"

player_t s_players[NUM_MAX_PLAYERS];
save_t s_saves[NUM_MAX_PLAYERS][NUM_MAX_SAVES_PER_PLAYER];
ending_t s_endings[NUM_MAX_PLAYERS][NUM_MAX_ENDINGS_PER_PLAYER];

int players_num;
CRITICAL_SECTION CriticalSection;

void InitGame()
{
	InitializeCriticalSectionAndSpinCount(&CriticalSection, 2000);

	FILE* pb = fopen("player.bin", "rb");

	if (pb == NULL) {
		puts("파일오픈 실패");
		return;
	}

	players_num = fread(s_players, sizeof(player_t), NUM_MAX_PLAYERS, pb);
}

int Game(SOCKET sock)
{
	EnterCriticalSection(&CriticalSection);

	int msg_int;
	int r_num;

START:

	msg_int = 1;

	while(msg_int != -1) {

	recv(sock, &msg_int, sizeof(msg_int), 0);

	switch (msg_int) {

	case SIGNUP: {
		msg_int = SignUp(sock, &players_num);
		if (msg_int == 0) {
			goto EXIT;
		}

		break;
	}

	case LOGIN: {
		r_num = Login(sock, players_num);
		if (r_num == -2) {
			goto EXIT;
		}

		msg_int = -1;
		break;
	}

	}
	}

	while (true) {

		recv(sock, &msg_int, sizeof(msg_int), 0);

		switch (msg_int) {

		case START_GAME: {
			break;
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
			break;
		}

		case OPTION: {
			recv(sock, &msg_int, sizeof(msg_int), 0);

			switch (msg_int) {
				
			case LOGIN_DATA: {
				send(sock, s_players[r_num].ID, MAX_MSG_LEN, 0);
				send(sock, s_players[r_num].password, MAX_MSG_LEN, 0);
				send(sock, &s_players[r_num].p_num, sizeof(int), 0);
				send(sock, &s_players[r_num].e_num, sizeof(int), 0);

				break;
			}

			case LOGOUT: {
				goto START;
			}

			case BACK: {
				break;
			}
			}

			break;
		}

		case ENDING: {

			int ending_existence;

			for (int i = 0; i < NUM_MAX_ENDINGS_PER_PLAYER; ++i) {

				for (int j = 0; j < s_players[r_num].e_num; ++j) {

					if (s_endings[r_num][j].number == i) {
						ending_existence = j;
						break;
					}

					else {
						ending_existence = 0;
					}
				}

				send(sock, &ending_existence, sizeof(ending_existence), 0);

				if (ending_existence != 0) {
					send(sock, s_endings[r_num][ending_existence].name, MAX_MSG_LEN, 0);
				}

			}

			break;
		}

		case EXIT: {
			goto EXIT;
		}

		}
	}

	EXIT: LeaveCriticalSection(&CriticalSection);
}

void CloseGame()
{
	DeleteCriticalSection(&CriticalSection);
}
