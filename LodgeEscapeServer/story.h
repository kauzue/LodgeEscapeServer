#pragma once

int Chapter1_Player1(SOCKET, int);
int Chapter1_Player2(SOCKET, int);

enum MENU { ITEM, CLUE, OPTION_MENU, BACK_MENU, EXIT_MENU };
enum STAGE1 { EXPLORE, INVESTIGATE, MENU };