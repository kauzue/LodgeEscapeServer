#pragma warning(disable:4996)

#include <winsock2.h>
#include <stdbool.h>

#include "game.h"
#include "story.h"

player_t s_players[NUM_MAX_PLAYERS];
room_t s_rooms[NUM_MAX_ROOMS];
item_t s_items[NUM_MAX_ITEMS];
clue_t s_clues[NUM_MAX_CLUES];

int num_items;
int num_clues;

int Menu(SOCKET sock, int r_p_num)
{
	int msg_int;

	recv(sock, &msg_int, sizeof(msg_int), 0);

	switch (msg_int) {
	case ITEM: {
		send(sock, &num_items, sizeof(num_items), 0);

		if (num_clues == 0) {
			return 0;
		}

		for (int i = 0; i < num_items; i++) {
			send(sock, s_items[i].name, sizeof(s_items[i].name), 0);
			send(sock, s_items[i].information, sizeof(s_items[i].information), 0);
		}
		break;
	}

	case CLUE: {
		send(sock, &num_clues, sizeof(num_clues), 0);

		if (num_clues == 0) {
			return 0;
		}

		for (int i = 0; i < num_clues; i++) {
			send(sock, s_clues[i].name, sizeof(s_clues[i].name), 0);
			send(sock, s_clues[i].information, sizeof(s_clues[i].information), 0);
		}
		break;
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

		case BACK: {
			return 0;
		}

		}

	}

	case BACK_MENU: {
		return 0;
	}

	case EXIT_MENU: {
		return -1;
	}

	}
	
	return 1;
}

void Exit_Wait_Room(SOCKET sock, int num_players)
{
	num_players += num_players;

	while (true) {
		if (num_players == 2) {
			break;
		}
	}
}

int Chapter1_Player1(SOCKET sock, int r_p_num)
{
	int msg_int;
	int explore = 0;
	int investigate = 0;
	int menu = 1;

	item_t item;

	num_items = 0;
	num_clues = 0;

	while (explore < 1 || investigate < 1) {
		recv(sock, &msg_int, sizeof(msg_int), 0);

		switch (msg_int) {

		case EXPLORE: {
			explore++;
			break;
		}

		case INVESTIGATE: {
			strcpy(item.name, "손전등");
			strcpy(item.information, "현재 배터리가 없어 작동되지 않는 것 같다.");
			memcpy(&s_items[num_items], &item, sizeof(item_t));
			++(num_items);

			strcpy(item.name, "지갑");
			strcpy(item.information, "지갑 안에는 돈 2300원 밖에 남아있지않다.");
			memcpy(&s_items[num_items], &item, sizeof(item_t));
			++(num_items);

			strcpy(item.name, "수첩");
			strcpy(item.information, "빈 종이가 100개 있다.");
			memcpy(&s_items[num_items], &item, sizeof(item_t));
			++(num_items);

			investigate++;
			break;
		}

		case MENU: {
			do {
				menu = Menu(sock, r_p_num);

				if (menu == -1) {
					Exit_Wait_Room(sock, 1);
					return r_p_num;
				}

			} while (menu <= 0);
			break;
		}

		}

	}
}

int Chapter1_Player2(SOCKET sock, int r_p_num)
{
	return r_p_num;
}