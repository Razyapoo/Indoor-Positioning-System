#ifndef SERVER_H
#define SERVER_H

/*********************************************** Server (Centralized) ******************************************
 * Communicates with UWB Tags and collected data from them.
 * Implements centralized architecture: First listen for new tags, then handle each tag one after another,
 *   initiating the communication. 
 * 
 * Executed in a separated thread (in main.cpp) - to not block Video data collection
 * 
****************************************************************************************************************/

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

#include "SharedData.h"

class Server
{
public:
    // Server settings
    static int serverSocketFD, clientSocketFD, currentClientSocketFD;
    static int maxSocketFD;
    static struct sockaddr_in serverAddress, clientAddress;
    static std::vector<int> clientSocketList;
    static socklen_t clientAddrLength;
    static char buffer[1024];
    static int opt;
    static size_t nbytes;
    static const size_t MAX_CLIENTS;
    static fd_set readFDS, tmpFDS;
    
    static struct timeval timeout; // set timeout in case tag is not respoding long time

    // Communication with tags: collecting the UWB data and timestamp of its receipt
    static std::chrono::milliseconds currentTime;
    static std::chrono::time_point<std::chrono::high_resolution_clock> requestTime, responseTime;
    static std::time_t timestamp;
    static size_t dataIndex; // identifier of UWB record
    static bool isBusy; // busy communicating with a tag

    // Attributes for the window to show if UWB data stream is active, or there is somewhere blocked communication
    static int statusImageWidth, statusImageHeight;
    static int activity;

    // Main queue where the server puts anchors to process next
    // and selects another tag to work with
    static std::queue<int> clientQueue;
    
    static void runServer();
    static void printFDSet(fd_set *set);
    static void checkForActive();

    static bool debugMode;
};

#endif