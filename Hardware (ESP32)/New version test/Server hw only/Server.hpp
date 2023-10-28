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
#include <vector>
#include <future>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>

class Server
{
public:
    static int serverSocketFD, clientSocketFD, currentClientSocketFD;
    static int maxSocketFD;
    static struct sockaddr_in serverAddress, clientAddress;
    static std::vector<int> clientSocketList;
    static socklen_t clientAddrLength;
    static char buffer[1024];
    static int opt;
    static int activity;
    static size_t nbytes;
    static const size_t MAX_CLIENTS;
    static std::chrono::milliseconds currentTime;
    static std::chrono::time_point<std::chrono::high_resolution_clock> requestTime, responseTime;
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