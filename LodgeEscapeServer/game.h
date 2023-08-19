#pragma once

#define NUM_MAX_PLAYERS 100
#define NUM_MAX_ROOMS 10
#define NUM_MAX_SAVES_PER_PLAYER 20
#define NUM_MAX_ENDINGS_PER_PLAYER 5
#define MAX_MSG_LEN 256

typedef struct player {
	char ID[MAX_MSG_LEN];
	char password[MAX_MSG_LEN];
	int p_num;
	int s_num;
	int e_num;
} player_t;

typedef struct room {
	char r_name[MAX_MSG_LEN];
	int r_password;
	int player_num;
} room_t;

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

enum LOG { SIGNUP, LOGIN};
enum MAIN { START_GAME, LOAD_GAME, OPTION, ENDING, EXIT };
enum ROOM {CREATE, FIND};
enum OPTION { LOGIN_DATA, LOGOUT, BACK };

void InitGame();
int Game(SOCKET);
void CloseGame();