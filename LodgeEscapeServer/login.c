#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <Windows.h>

#include "game.h"
#include "login.h"

player_t s_players[NUM_MAX_PLAYERS];

void SignUp(SOCKET sock, int* s_num_players)
{
	FILE* pb = fopen("player.bin", "ab");
	if (pb == NULL) {
		puts("파일오픈 실패!");
		return;
	}

	char msg_char[MAX_MSG_LEN] = "";
	int same = 0;

	player_t player;
	do {
		same = 0;

		recv(sock, msg_char, MAX_MSG_LEN, 0);

		for (int i = 0; i < *s_num_players; ++i) {
			if (strcmp(msg_char, s_players[i].ID) == 0) {
				same = 1;
				break;
			}
		}

		send(sock, &same, sizeof(same), 0);
	} while (same);

	strcpy(player.ID, msg_char);

	recv(sock, msg_char, MAX_MSG_LEN, 0);
	strcpy(player.password, msg_char);

	do {
		recv(sock, &player.p_num, sizeof(player.p_num), 0);

		if (player.p_num != 1 && player.p_num != 2) {
			same++;
			send(sock, &same, sizeof(same), 0);
		}

		else {
			same = 0;
			send(sock, &same, sizeof(same), 0);
		}
	} while (same);

	player.s_num = 0;

	memcpy(&s_players[*s_num_players], &player, sizeof(player_t));
	++(*s_num_players);

	fwrite(&player, sizeof(player_t), 1, pb);

	fclose(pb);
}

int Login(SOCKET sock, int s_num_players)
{
	int r_num;
	int b_login = 0;

	char ID[MAX_MSG_LEN] = "";
	char Password[MAX_MSG_LEN] = "";

	while (!b_login) {
		recv(sock, &ID, MAX_MSG_LEN, 0);
		recv(sock, &Password, MAX_MSG_LEN, 0);

		for (r_num = 0; r_num < s_num_players; ++r_num) {
			if (strcmp(ID, s_players[r_num].ID) == 0 && strcmp(Password, s_players[r_num].password) == 0) {
				b_login = s_players[r_num].p_num;
				break;
			}
		}

		send(sock, &b_login, sizeof(b_login), 0);
	}

	return r_num;
}