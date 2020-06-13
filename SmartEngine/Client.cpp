#include "Client.h"
volatile int id2 ;
volatile int kils2;
void Client::UpdateScreenTCP(LPVOID server_addr)
{
    SOCKADDR_IN recvaddr; //= ((SOCKADDR_IN*)server_addr)[0];
    int iaddrLen = sizeof(recvaddr);
    char chMessage[BUFLEN];
    int iMesLen = strlen(chMessage);
    char chRecData[255];
    int ret;
    memset((char*)&recvaddr, 0, sizeof(recvaddr));
    recvaddr.sin_family = AF_INET;
    recvaddr.sin_port = htons(iPort);
    recvaddr.sin_addr.S_un.S_addr = inet_addr(SERVER);
    if (recvaddr.sin_addr.S_un.S_addr == INADDR_NONE)
    {
        host = NULL;
        printf("Resolving host...\n");
        host = gethostbyname(chAddress);

        if (host == NULL)
        {
            printf("Unknown host: %s\n", chAddress);
            exit(1);
        }
        memcpy(&recvaddr.sin_addr, host->h_addr_list[0], host->h_length);
        printf("OK\n");
    }
    printf("Connecting to %s:%d", chAddress, iPort);
    ret = connect(sock, (SOCKADDR*)&recvaddr, sizeof(recvaddr));
    if (ret == SOCKET_ERROR)
    {
        printf("connect() failed with error code %ld\n", WSAGetLastError());
        return;
    }
    else
        printf("connect() is OK!\n");
    // Инициализация закончена, тперь можно отправлять сообщения
    // Получаем наш id от сервера
    ret = recv(sock, chMessage, BUFLEN, 0);
    cp.id = atoi(chMessage);
    plrs.push_back(cp);
    //Отправляем x, y, z серверу
    std::string mess1 = std::to_string(plrs[0].x);
    std::string mess2 = std::to_string(plrs[0].y);
    std::string mess3 = std::to_string(plrs[0].z);
    sendt(mess1.c_str());
    sendt(mess2.c_str());
    sendt(mess3.c_str());

    memset(chMessage, '\0', BUFLEN);
    while (1)
    {
        
        //fflush(stdout);
        mess1 = std::to_string(plrs[0].x);
        mess2 = std::to_string(plrs[0].y);
        mess3 = std::to_string(plrs[0].z);
        sendt(mess1.c_str());
        sendt(mess2.c_str());
        sendt(mess3.c_str());
        player np;
        memset(chMessage, '\0', BUFLEN);
        ret = recv(sock, chMessage, BUFLEN, 0);
        np.id = atoi(chMessage);

        memset(chMessage, '\0', BUFLEN);
        ret = recv(sock, chMessage, BUFLEN, 0);
        np.x = atoi(chMessage);

        memset(chMessage, '\0', BUFLEN);
        ret = recv(sock, chMessage, BUFLEN, 0);
        np.y = atoi(chMessage);
        memset(chMessage, '\0', BUFLEN);

        ret = recv(sock, chMessage, BUFLEN, 0);
        np.z = atoi(chMessage);
        // Новый игрок?
        bool nsk = true;
        for (int i = 0; i < plrs.size(); i++)
        {
            if (plrs[i].id == np.id && np.id!=cp.id)
            {
                nsk = false;
                plrs[i].x = np.x;
                plrs[i].y = np.y;
                plrs[i].z = np.z;
            }
        }
        if (nsk && np.id != cp.id)
        {
            plrs.push_back(np);
        }
        //const char* mess1 = std::to_string(players[i].id).c_str();
        //const char* mess2 = std::to_string(players[i].x).c_str();
        //const char* mess3 = std::to_string(players[i].y).c_str();
        //const char* mess4 = std::to_string(players[i].z).c_str();
        //send(sClient, mess1, sizeof(chSendBuffer), 0);
        //send(sClient, mess2, sizeof(chSendBuffer), 0);
        //send(sClient, mess3, sizeof(chSendBuffer), 0);
        //send(sClient, mess4, sizeof(chSendBuffer), 0);
    }
    printf("%s\n", chMessage);
    cp.id = atoi(chMessage);
    plrs[0].id = cp.id;
    memset(chMessage, '\0', BUFLEN);
    /*recvfrom(sock, chMessage, BUFLEN, 0, (struct sockaddr*) & recvaddr, &iaddrLen);
    printf("%s\n", chMessage);

    cp.x = atoi(chMessage);
    memset(chMessage, '\0', BUFLEN);
    recvfrom(sock, chMessage, BUFLEN, 0, (struct sockaddr*) & recvaddr, &iaddrLen);
    printf("%s\n", chMessage);

    cp.y = atoi(chMessage);
    memset(chMessage, '\0', BUFLEN);
    recvfrom(sock, chMessage, BUFLEN, 0, (struct sockaddr*) & recvaddr, &iaddrLen);
    printf("%s\n", chMessage);

    cp.z = atoi(chMessage);
    memset(chMessage, '\0', BUFLEN);
    recvfrom(sock, chMessage, BUFLEN, 0, (struct sockaddr*) & recvaddr, &iaddrLen);
    printf("%s\n", chMessage);

    cp.health = atoi(chMessage);
    memset(chMessage, '\0', BUFLEN);
    recvfrom(sock, chMessage, BUFLEN, 0, (struct sockaddr*) & recvaddr, &iaddrLen);
    printf("%s\n", chMessage);

    cp.kills = atoi(chMessage);
    memset(chMessage, '\0', BUFLEN);
    recvfrom(sock, chMessage, BUFLEN, 0, (struct sockaddr*) & recvaddr, &iaddrLen);
    printf("%s\n", chMessage);

    cp.deaths = atoi(chMessage);*/
    //plrs.push_back(cp);
    /*for (int i = 0; i < players.size(); i++)
    {
        if (players[i]->id == cp.id)
        {
            players[i]->reposition_body(btVector3(cp.x, cp.y, cp.z));
            players[i]->set_kills(cp.kills);
            players[i]->set_deaths(cp.deaths);
            players[i]->set_health(cp.health);
        }
    }*/
    //while (1)
    //{
    //    player np;
    //    memset(chMessage, '\0', BUFLEN);
    //    //try to receive some data, this is a blocking call
    //    if (recvfrom(sock, chMessage, BUFLEN, 0, (struct sockaddr*) & recvaddr, &iaddrLen) == SOCKET_ERROR)
    //    {
    //        printf("recvfrom() failed with error code : %ld ", WSAGetLastError());
    //        //return;// exit(EXIT_FAILURE);
    //    }
    //    np.id = atoi(chMessage);

    //    memset(chMessage, '\0', BUFLEN);
    //    recvfrom(sock, chMessage, BUFLEN, 0, (struct sockaddr*) & recvaddr, &iaddrLen);
    //    np.x = atoi(chMessage);
    //    memset(chMessage, '\0', BUFLEN);
    //    recvfrom(sock, chMessage, BUFLEN, 0, (struct sockaddr*) & recvaddr, &iaddrLen);
    //    np.y = atoi(chMessage);
    //    memset(chMessage, '\0', BUFLEN);
    //    recvfrom(sock, chMessage, BUFLEN, 0, (struct sockaddr*) & recvaddr, &iaddrLen);
    //    np.z = atoi(chMessage);
    //    /*memset(chMessage, '\0', BUFLEN);
    //    recvfrom(sock, chMessage, BUFLEN, 0, (struct sockaddr*) & recvaddr, &iaddrLen);
    //    np.health = atoi(chMessage);
    //    memset(chMessage, '\0', BUFLEN);
    //    recvfrom(sock, chMessage, BUFLEN, 0, (struct sockaddr*) & recvaddr, &iaddrLen);
    //    np.kills = atoi(chMessage);
    //    memset(chMessage, '\0', BUFLEN);
    //    recvfrom(sock, chMessage, BUFLEN, 0, (struct sockaddr*) & recvaddr, &iaddrLen);
    //    np.deaths = atoi(chMessage);*/
    //    bool finded = false;

    //    for (int i = 0; i < plrs.size(); i++)
    //    {
    //        if (np.id == plrs[i].id) {
    //            finded = true;
    //            plrs[i] = np;
    //        }
    //    }
    //    if (!finded)
    //    {
    //        plrs.push_back(np);
    //    }
    //    //plrs[np.id] = np;
    //    /*for (int i = 0; i < plrs.size(); i++)
    //    {
    //        if (plrs[i].id == np.id)
    //        {

    //            plrs[i].reposition_body(btVector3(np.x, np.y, np.z));
    //            plrs[i].set_kills(np.kills);
    //            plrs[i].set_deaths(np.deaths);
    //            plrs[i].set_health(np.health);
    //        }
    //    }
    //    if (!finded)
    //    {
    //        Player *newp = new Player("Bambi",glm::vec3(np.x,np.y,np.z));
    //        plrs.push_back(newp);
    //    }*/
        printf("\n%s\n", chMessage);
        printf("Me: ");
    

    return;
}

void Client::UpdateTCP(LPVOID server_socket)
{
    SOCKET sock = ((SOCKET*)server_socket)[0];
    char chRecData[255];
    int ret;
    while (1) {
        ret = recv(sock, chRecData, sizeof(chRecData), 0);

        if (ret == SOCKET_ERROR)
        {
            printf("recv() failed with error code %ld\n", WSAGetLastError());
            if (WSAGetLastError() == WSAECONNRESET)
            {
                printf("connection is closed by peer, closing programm...\n");
                exit(1);
            }
        }
        else
        {
            // printf("recv()");
        }
        int numb = atoi(chRecData);
        //chRecData[ret] = '\0';
        int id = numb / 100;
        int kils = numb % 100;
        id2 = id;
        kils2 = kils;
        printf("\n%s\n", chRecData);
        //printf("Me: ");
    }
    return;
}
Client::Client() {
	init();
}

Client::~Client() { close(); }

void Client::init()
{
    if (WSAStartup(MAKEWORD(2, 2), &wsdata) != 0)
    {
        printf("WSAStartup() failed with error code %ld\n", WSAGetLastError());
        return;
    }
    else
    {
        printf("WSAStartup() is OK!\n");
    }
    strcpy(chAddress, "127.0.0.1");
	addrlen = sizeof(addr);
    iPort = 1111;
    if (iPort < 0 || iPort> 65563)
        printf("Invalid port number %d", iPort);

    SOCKADDR_IN addr; // The host's address
    hostent* host;
    char chMessage[255] = "Hello woooorld!Reeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee";
    std::string sMessage;
    char chRecData[255];
    int iMesLen;
    int ret;

    //create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == SOCKET_ERROR)
    {
        printf("socket() failed with error code : %ld", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    iMesLen = strlen(chMessage);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(iPort);
    addr.sin_addr.S_un.S_addr = inet_addr(chAddress);

    if (addr.sin_addr.S_un.S_addr == INADDR_NONE)
    {
        host = NULL;
        printf("Resolving host...\n");
        host = gethostbyname(chAddress);

        if (host == NULL)
        {
            printf("Unknown host: %s\n", chAddress);
            exit(1);
        }
        memcpy(&addr.sin_addr, host->h_addr_list[0], host->h_length);
        printf("OK\n");
    }
    printf("Connecting to %s:%d", chAddress, iPort);
    ret = connect(sock, (SOCKADDR*)&addr, sizeof(addr));
    if (ret == SOCKET_ERROR)
    {
        printf("connect() failed with error code %ld\n", WSAGetLastError());
        return;
    }
    else
        printf("connect() is OK!\n");
    // Получаем свой id
    ret = recv(sock, chRecData, sizeof(chRecData), 0);
    id1 = atoi(chRecData);
    some_threads.push_back(std::thread(&Client::UpdateTCP, this, &sock));
    /*DWORD thID;
    CreateThread(NULL, 0, UpdateTCP, &sock, 0, &thID);*/

    //while (1)
    //{
    //    //printf("Me: ");
    //    

    //    if (ret == SOCKET_ERROR)
    //    {
    //        printf("send() failed with error code %ld\n", WSAGetLastError());
    //    }
    //    else
    //    {
    //        //printf("send() is OK!\n");
    //    }
    //}
    ////create socket
    //if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == SOCKET_ERROR)
    //{
    //    printf("socket() failed with error code : %ld", WSAGetLastError());
    //    exit(EXIT_FAILURE);
    //}

    ////setup address structure
    //memset((char*)&addr, 0, sizeof(addr));
    //addr.sin_family = AF_INET;
    //addr.sin_port = htons(iPort);
    //addr.sin_addr.S_un.S_addr = inet_addr(SERVER);
    ///*if (sendto(sock, chMessage, strlen(chMessage), 0, (struct sockaddr*) & addr, addrlen) == SOCKET_ERROR)
    //{
    //    printf("sendto() failed with error code : %ld", WSAGetLastError());
    //    exit(EXIT_FAILURE);
    //}*/
    //some_threads.push_back(std::thread(&Client::UpdateScreenTCP, this,&myaddr));
    ////update_thread = new std::thread(this->UpdateScreenUDP);
    ////CreateThread(NULL, 0,UpdateScreenUDP, &myaddr, 0, &thID);
}

void Client::close() {
    for (int i=0; i<some_threads.size();i++)
    {
        some_threads[i].join();
    }
    if (closesocket(sock) == SOCKET_ERROR)
    {
        printf("closesocket() failed with error code %ld\n", WSAGetLastError());
    }
    else
    {
        printf("closesocket() is OK!\n");
    }
    WSACleanup();
}

void Client::sendt(const char* mess)
{
    //send the message
    if (send(sock, mess, strlen(mess), 0) == SOCKET_ERROR)
    {
        printf("send() failed with error code : %ld", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
}
