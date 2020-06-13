#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <io.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <WinSock2.h>
#include <WSipx.h>
#include <WSNwLink.h>
#include <Windows.h>
#include <thread>
#include <vector>


#define MAX_CONNECTED_SOCKETS 10
#define BUFLEN 512
#define SERVER "127.0.0.1"


class Client
{
public:
    struct player {
        int id;
        int x = 0.f;
        int y = 100.f;
        int z = 0.f;
        int health = 100;
        int kills = 100;
        int deaths = 100;
    };
    Client();
    ~Client();
    void sendt(const char* mess);
    std::vector<player> plrs;
    int player_c;
    int id1=0;
    int kils1 = 0;
    int id22 = 0;
    int kils22 = 0;
private:
    void UpdateScreenTCP(LPVOID server_addr);
    void UpdateTCP(LPVOID server_socket);
    WSADATA wsdata;
    SOCKADDR_IN addr; // The host's address
    SOCKADDR_IN myaddr;
    SOCKADDR_IN recvaddr;
    hostent* host;
    char chMessage[BUFLEN] = "port connected to the chat\0";
    std::string sMessage;
    char chRecData[255];
    char chMyAddr[255];
    int iMesLen;
    int ret;
    int addrlen;
    BOOL bServer = FALSE;
    SOCKET sock;
    player cp;
    DWORD thID;
    std::vector<std::thread> some_threads;
    int socket_count = 0;
    int iPort;
    int iAddrLen;
    char chAddress[64];
    void init();
    void close();
};