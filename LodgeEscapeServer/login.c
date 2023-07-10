#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <Windows.h>

#include "game.h"
#include "login.h"

player_t s_players[NUM_MAX_PLAYERS];

void SignUp(SOCKET sock, int s_num_players)
{
	FILE* pb = fopen("player.bin", "ab");
	if (pb == NULL) {
		puts("파일오픈 실패!");
		return;
	}

	int same = 0;
	char msg_char[MAX_MSG_LEN] = "";

	player_t player;
	do {
		recv(sock, msg_char, MAX_MSG_LEN, 0);
		send(sock, &s_num_players, sizeof(s_num_players), 0);

		for (int i = 0; i < s_num_players; ++i) {
			if (strcmp(msg_char, s_players[i].ID) == 0) {
				same++;
				send(sock, &same, sizeof(same), 0);
			}

			else {
				same = 0;
				send(sock, &same, sizeof(same), 0);
			}
		}
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

	memcpy(&s_players[s_num_players], &player, sizeof(player_t));
	++s_num_players;

	fwrite(&player, sizeof(player_t), 1, pb);

	fclose(pb);
}

void Login()
{

}