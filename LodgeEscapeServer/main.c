#pragma warning(disable:4996)

#include "winsock_server.h"
#include "game.h"

CRITICAL_SECTION Cs;

int main()
{
	InitializeCriticalSectionAndSpinCount(&Cs, 4000);
	//InitGame();
	StartWinsock(Cs);

	DeleteCriticalSection(&Cs);
}