#include "winsock_server.h"
#include "game.h"

int main()
{
	InitGame();
	StartWinsock();
	return 0;
}