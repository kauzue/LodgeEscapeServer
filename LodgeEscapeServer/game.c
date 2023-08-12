#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <winsock2.h>
#include <stdbool.h>

#include "game.h"
#include "login.h"

player_t s_players[NUM_MAX_PLAYERS];
room_t s_rooms[NUM_MAX_ROOMS];
save_t s_saves[NUM_MAX_PLAYERS][NUM_MAX_SAVES_PER_PLAYER];
ending_t s_endings[NUM_MAX_PLAYERS][NUM_MAX_ENDINGS_PER_PLAYER];

int players_num;
int rooms_num;
CRITICAL_SECTION CriticalSection;

void CreateRoom(SOCKET);
void FindRoom(SOCKET);

void InitGame()
{
	InitializeCriticalSectionAndSpinCount(&CriticalSection, 2000);

	FILE* pb = fopen("player.bin", "rb");
	FILE* rb = fopen("room.bin", "rb");

	if (pb == NULL) {
		puts("플레이어 파일 오픈 실패");
		return;
	}

	if (rb == NULL) {
		puts("방 파일 오픈 실패");
		fclose(pb);
		return;
	}

	players_num = fread(s_players, sizeof(player_t), NUM_MAX_PLAYERS, pb);
	rooms_num = fread(s_rooms, sizeof(room_t), NUM_MAX_ROOMS, rb);

	fclose(pb);
	fclose(rb);
}

int Game(SOCKET sock)
{
	EnterCriticalSection(&CriticalSection);

	int msg_int;
	int r_num;
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
				send(sock, &s_players[r_num].s_num, sizeof(int), 0);

				if (s_players[r_num].s_num != 0) {
					msg_int = 0;

					for (msg_int; msg_int < s_players[r_num].s_num; ++msg_int) {
						send(sock, &s_saves[r_num][msg_int].chapter, NUM_MAX_SAVES_PER_PLAYER, 0);
						send(sock, &s_saves[r_num][msg_int].stage, NUM_MAX_SAVES_PER_PLAYER, 0);
					}

					recv(sock, &r_chapter_stage_num, sizeof(r_chapter_stage_num), 0);
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
	START:;

	}

EXIT: LeaveCriticalSection(&CriticalSection);
}

void CreateRoom(SOCKET sock)
{
	FILE* rb = fopen("room.bin", "ab");
	if (rb == NULL) {
		puts("파일오픈 실패!");
		return;
	}

	char msg_char[MAX_MSG_LEN] = "";
	int msg_int;
	int same = 1;

	room_t room;
	while (same > 0) {
		recv(sock, msg_char, MAX_MSG_LEN, 0);
		if (!strcmp(msg_char, "0")) {
			same = -1;
			send(sock, &same, sizeof(same), 0);
			goto END_CREATEROOM;
		}

		if (rooms_num == 0) {
			same = 0;
		}

		for (int i = 0; i < rooms_num; ++i) {
			if (strcmp(msg_char, s_rooms[i].r_name) == 0) {
				same = 1;
				break;
			}

			else {
				same = 0;
			}
		}
		send(sock, &same, sizeof(same), 0);
	};

	strcpy(room.r_name, msg_char);

	recv(sock, &msg_int, sizeof(msg_int), 0);
	room.r_password = msg_int;
	send(sock, &rooms_num, sizeof(rooms_num), 0);

	memcpy(&s_rooms[rooms_num], &room, sizeof(room_t));
	++(rooms_num);

	fwrite(&room, sizeof(room_t), 1, rb);

	fclose(rb);

END_CREATEROOM:
	fclose(rb);
}

void FindRoom(SOCKET sock)
{
	send(sock, &rooms_num, sizeof(rooms_num), 0);
}

void CloseGame()
{
	DeleteCriticalSection(&CriticalSection);
}
