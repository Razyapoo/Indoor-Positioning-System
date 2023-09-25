#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <string>
#include <queue>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <unistd.h>
#include <fstream>
#include <chrono>
#include <thread>

class Server
{
public:
    static int serverSocketFD, clientSocketFD;
    static struct sockaddr_in serverAddress, clientAddress;
    static socklen_t clientLength;
    static char buffer[1024];
    static std::chrono::milliseconds currentTime;
    static std::time_t timestamp;
    static size_t dataIndex;
    static bool isBusy;

    static std::queue<int> clientQueue;
    static fd_set readFDS, tmpFDS;
    static struct timeval timeout;

    static void runServer();
    static void printFDSet(fd_set *set);
    static void showQueue(std::queue<int> queue);

    static bool debugMode;
};

#endif