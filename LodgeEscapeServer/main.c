#include "winsock_server.h"
#include "game.h"

int main()
{
	InitGame();
	StartWinsock();

	CloseGame();
	return 0;
}