#pragma once

#define NUM_MAX_PLAYERS 100
#define NUM_MAX_SAVES_PER_PLAYER 20
#define MAX_MSG_LEN 256

typedef struct player {
	char ID[MAX_MSG_LEN];
	char password[MAX_MSG_LEN];
	int p_num;
} player_t;

typedef struct save {
	int stage;
	int chapter;
} save_t;

typedef struct ending {
	int number;
	char name[MAX_MSG_LEN];
} ending_t;

typedef struct clue {
	char name[MAX_MSG_LEN];
	char information[MAX_MSG_LEN];
} clue_t;

void loginmenu(SOCKET, int);