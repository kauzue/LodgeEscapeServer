#pragma once
#pragma comment(lib,"ws2_32")
#pragma warning(disable:4996)

#include <WinSock2.h>
#include <stdbool.h>

#define PORT_NUM 12345
#define BLOG_SIZE 5

SOCKET StartWinsock();
IN_ADDR GetDefaultMyIP();
SOCKET SetTCPServer(short, int);