#include <winsock2.h>]

#include "game.h"
#include "story.h"

player_t s_players[NUM_MAX_PLAYERS];

void Menu(SOCKET sock, int r_p_num)
{
	int msg_int;

	recv(sock, &msg_int, sizeof(msg_int), 0);

	switch (msg_int) {
	case ITEM: {

	}

	case CLUE: {

	}

	case OPTION_MENU: {
		recv(sock, &msg_int, sizeof(msg_int), 0);

		switch (msg_int) {

		case LOGIN_DATA: {
			send(sock, s_players[r_p_num].ID, MAX_MSG_LEN, 0);
			send(sock, s_players[r_p_num].password, MAX_MSG_LEN, 0);
			send(sock, &s_players[r_p_num].p_num, sizeof(int), 0);
			send(sock, &s_players[r_p_num].e_num, sizeof(int), 0);

			break;
		}

		case BACK_MENU: {

		}

		case EXIT_MENU: {

		}

		}
	}

	}
}

void Chapter1_Player1(SOCKET sock, int r_p_num)
{
	int msg_int;
	int explore = 0;
	int investigate = 0;

	while (explore < 1 || investigate < 1) {
		recv(sock, &msg_int, sizeof(msg_int), 0);

		switch (msg_int) {

		case EXPLORE: {
			explore++;
		}

		case INVESTIGATE: {
			investigate++;
		}

		case MENU: {
			Menu(sock, r_p_num);
		}

		}

	}
}

void Chapter1_Player2(SOCKET sock, int r_p_num)
{

}