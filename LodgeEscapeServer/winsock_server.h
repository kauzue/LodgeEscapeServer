#pragma once
#pragma comment(lib,"ws2_32")
#pragma warning(disable:4996)

#include <WinSock2.h>
#include <stdbool.h>

#define PORT_NUM 1252
#define BLOG_SIZE 5

void StartWinsock(CRITICAL_SECTION);
IN_ADDR GetDefaultMyIP();
SOCKET SetTCPServer(short, int);