#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <winsock2.h>
#include <stdbool.h>

#include "game.h"
#include "login.h"
#include "story.h"

player_t s_players[NUM_MAX_PLAYERS];
room_t s_rooms[NUM_MAX_ROOMS];
save_t s_saves[NUM_MAX_PLAYERS][NUM_MAX_SAVES_PER_PLAYER];
ending_t s_endings[NUM_MAX_PLAYERS][NUM_MAX_ENDINGS_PER_PLAYER];

CRITICAL_SECTION CriticalSection;

int players_num;
int r_p_num;
int r_r_num;
int rooms_num = 0;

void CreateRoom(SOCKET);
void FindRoom(SOCKET);
void WaitRoom(SOCKET);

void InitGame()
{
	InitializeCriticalSectionAndSpinCount(&CriticalSection, 2000);

	FILE* pb = fopen("player.bin", "rb");

	if (pb == NULL) {
		puts("플레이어 파일 오픈 실패");
		return;
	}

	players_num = fread(s_players, sizeof(player_t), NUM_MAX_PLAYERS, pb);

	fclose(pb);
}

int Game(SOCKET sock)
{
	int msg_int;
	int r_chapter_stage_num;

	// 위로 점프 금지

	while (true) {
		msg_int = 1;

		while (msg_int != -1) {

			recv(sock, &msg_int, sizeof(msg_int), 0);

			switch (msg_int) {

			case SIGNUP: {
				msg_int = SignUp(sock, &players_num);
				if (msg_int == 0) {
					return;
				}

				break;
			}

			case LOGIN: {
				r_p_num = Login(sock, players_num);
				if (r_p_num == -2) {
					return;
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
				recv(sock, &msg_int, sizeof(msg_int), 0);

				switch (msg_int) {
				case CREATE: {
					CreateRoom(sock);
					break;
				}

				case FIND: {
					FindRoom(sock);
					break;
				}

				}
				break;
			}

			case LOAD_GAME: {
				send(sock, &s_players[r_p_num].s_num, sizeof(int), 0);

				if (s_players[r_p_num].s_num != 0) {
					msg_int = 0;

					for (msg_int; msg_int < s_players[r_p_num].s_num; ++msg_int) {
						send(sock, &s_saves[r_p_num][msg_int].chapter, NUM_MAX_SAVES_PER_PLAYER, 0);
						send(sock, &s_saves[r_p_num][msg_int].stage, NUM_MAX_SAVES_PER_PLAYER, 0);
					}

					recv(sock, &r_chapter_stage_num, sizeof(r_chapter_stage_num), 0);
				}

				break;
			}

			case OPTION: {
				recv(sock, &msg_int, sizeof(msg_int), 0);

				switch (msg_int) {

				case LOGIN_DATA: {
					send(sock, s_players[r_p_num].ID, MAX_MSG_LEN, 0);
					send(sock, s_players[r_p_num].password, MAX_MSG_LEN, 0);
					send(sock, &s_players[r_p_num].p_num, sizeof(int), 0);
					send(sock, &s_players[r_p_num].e_num, sizeof(int), 0);

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

					for (int j = 0; j < s_players[r_p_num].e_num; ++j) {

						if (s_endings[r_p_num][j].number == i) {
							ending_existence = j;
							break;
						}

						else {
							ending_existence = 0;
						}
					}

					send(sock, &ending_existence, sizeof(ending_existence), 0);

					if (ending_existence != 0) {
						send(sock, s_endings[r_p_num][ending_existence].name, MAX_MSG_LEN, 0);
					}

				}

				break;
			}

			case EXIT: {
				return;
			}

			}
		}
	START:;

	}
}

void CreateRoom(SOCKET sock)
{
	EnterCriticalSection(&CriticalSection);

	char msg_char[MAX_MSG_LEN] = "";
	int same = 0;

	room_t room;
	do {
		same = 0;

		recv(sock, msg_char, MAX_MSG_LEN, 0);
		if (!strcmp(msg_char, "0")) {
			same = -1;
			send(sock, &same, sizeof(same), 0);
			goto error_exit_create_room;
		}

		for (int i = 0; i < rooms_num; i++) {
			if (strcmp(msg_char, s_rooms[i].r_name) == 0) {
				same = 1;
				break;
			}
		}

		send(sock, &same, sizeof(same), 0);
	} while (same > 0);

	strcpy(room.r_name, msg_char);

	recv(sock, &room.r_password, sizeof(room.r_password), 0);

	room.player_num = s_players[r_p_num].p_num;

	memcpy(&s_rooms[rooms_num], &room, sizeof(room_t));

	r_r_num = rooms_num;

	++(rooms_num);

	send(sock, &r_p_num, sizeof(r_p_num), 0);

	LeaveCriticalSection(&CriticalSection);

	WaitRoom(sock);
	return;

error_exit_create_room:
	LeaveCriticalSection(&CriticalSection);

}

void FindRoom(SOCKET sock)
{
	EnterCriticalSection(&CriticalSection);

	int msg_int;

	send(sock, &rooms_num, sizeof(rooms_num), 0);

	if (rooms_num == 0) {
		goto error_not_fount_room;
	}

	for (int i = 0; i < rooms_num; i++) {
		send(sock, s_rooms[i].r_name, sizeof(s_rooms[i].r_name), 0);
	}

	recv(sock, &msg_int, sizeof(msg_int), 0);

	if (msg_int < 0 || msg_int > rooms_num) {
		goto error_not_fount_room;
	}

	else if (s_rooms[msg_int].player_num == s_players[r_p_num].p_num) {
		msg_int = 1;
		send(sock, &msg_int, sizeof(msg_int), 0);
		goto error_not_fount_room;
	}

	else {
		msg_int = 0;
		r_r_num = msg_int;
	}

	send(sock, &msg_int, sizeof(msg_int), 0);
	recv(sock, &msg_int, sizeof(msg_int), 0);
	if (msg_int == s_rooms[r_r_num].r_password) {
		msg_int = 1;
		send(sock, &msg_int, sizeof(msg_int), 0);
	}

	else {
		msg_int = 0;
		send(sock, &msg_int, sizeof(msg_int), 0);
		goto error_not_fount_room;
	}

	s_rooms[r_r_num].player_num += s_players[r_p_num].p_num;

	send(sock, &r_p_num, sizeof(r_p_num), 0);

	LeaveCriticalSection(&CriticalSection);

	WaitRoom(sock);
	return;

error_not_fount_room:
	LeaveCriticalSection(&CriticalSection);

}

void WaitRoom(SOCKET sock)
{
	int msg_int;
	int delete_rooms = 1;

	while (true) {
		if (s_rooms[r_r_num].player_num == 3) {
			break;
		}
	}
	send(sock, &s_players[r_p_num].p_num, sizeof(int), 0);
	recv(sock, &msg_int, sizeof(msg_int), 0);
	send(sock, &s_players[msg_int].p_num, sizeof(s_players[msg_int].p_num), 0);

	r_p_num = msg_int;

	if (delete_rooms == r_p_num) {
		for (int i = rooms_num; i > rooms_num - r_r_num; i--) {
			s_rooms[i - 1].player_num = s_rooms[i].player_num;
			strcpy(s_rooms[i - 1].r_name, s_rooms[i].r_name);
			s_rooms[i - 1].r_password = s_rooms[i].r_password;
		}
		rooms_num--;
	}


	if (s_players[r_p_num].p_num == 1) {
		r_p_num = Chapter1_Player1(sock, r_p_num);
	}

	else {
		r_p_num =  Chapter1_Player2(sock, r_p_num);
	}

}

void CloseGame()
{
	DeleteCriticalSection(&CriticalSection);
}
