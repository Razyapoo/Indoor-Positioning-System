#include "Server.hpp"

int Server::serverSocketFD, Server::clientSocketFD, Server::currentClientSocketFD;
int Server::maxSocketFD;
int Server::opt = 1;
int Server::activity;

fd_set Server::readFDS, Server::tmpFDS;

char Server::buffer[1024];
bool Server::isBusy = false;
size_t Server::dataIndex = 1;
const size_t Server::MAX_CLIENTS = 5;
size_t Server::nbytes;
std::vector<int> Server::clientSocketList(MAX_CLIENTS, 0);

struct sockaddr_in Server::serverAddress, Server::clientAddress;
struct timeval Server::timeout;

socklen_t Server::clientAddrLength;
std::queue<int> Server::clientQueue;
std::chrono::milliseconds Server::currentTime;
std::chrono::time_point<std::chrono::high_resolution_clock> Server::requestTime, Server::responseTime;
std::time_t Server::timestamp;

extern SharedData sharedData;

// DEBUG
bool Server::debugMode = true;

void Server::printFDSet(fd_set *set)
{
    std::cout << "fd_set { \n";
    for (size_t i = 0; i < MAX_CLIENTS; i++)
    {
        if (FD_ISSET(i, set))
        {
            std::cout << i << "\n";
        }
    }

    std::cout << "} \n";
}

void Server::runServer()
{
    // Create socket file descriptor
    if ((serverSocketFD = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket failed!");
        exit(EXIT_FAILURE);
    }

    // Helps to prevent the issue "address already in use"
    if (setsockopt(serverSocketFD, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("Failed to set setsockopt!");
        exit(EXIT_FAILURE);
    }

    // Attach socket to the port
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(30001);
    if (bind(serverSocketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        perror("Failed to bind");
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocketFD, 5) < 0)
    {
        perror("Failed to listen!");
        exit(EXIT_FAILURE);
    }

    std::ofstream timestampFile("timestamp_ESP32.txt");
    timestampFile.clear();
    if (!timestampFile.is_open())
        throw std::runtime_error("Failed to open timestamp_ESP32.txt file");

    clientAddrLength = sizeof(clientAddress);
    for (size_t socketID = 0; socketID < MAX_CLIENTS; socketID++)
    {
        clientSocketList[socketID] = 0;
    }

    while (true)
    {
        if (sharedData.terminationFlag())
        {
            for (size_t socketID = 0; socketID < MAX_CLIENTS; socketID++)
            {
                clientSocketFD = clientSocketList[socketID];
                close(clientSocketFD);
            }
            std::cout << "Server is closed" << std::endl;
            timestampFile.close();
            return;
        }

        // Clear socket set
        FD_ZERO(&readFDS);

        // add server socket to the set
        FD_SET(serverSocketFD, &readFDS);
        maxSocketFD = serverSocketFD;
        timeout.tv_sec = 20;

        // Add client sockets to set
        for (size_t socketID = 0; socketID < MAX_CLIENTS; socketID++)
        {
            clientSocketFD = clientSocketList[socketID];

            if (clientSocketFD > 0)
            {
                FD_SET(clientSocketFD, &readFDS);
            }

            if (clientSocketFD > maxSocketFD)
            {
                maxSocketFD = clientSocketFD;
            }
        }

        // std::cout << "Select phase. Max Socket FD: " << maxSocketFD << std::endl;
        activity = select(maxSocketFD + 1, &readFDS, NULL, NULL, &timeout);

        if ((activity < 0) && (errno != EINTR))
        {
            perror("Error during select");
        }

        if (FD_ISSET(serverSocketFD, &readFDS))
        {
            if ((clientSocketFD = accept(serverSocketFD, (struct sockaddr *)&clientAddress, (socklen_t *)&clientAddrLength)) < 0)
            {
                perror("Accept error!");
                exit(EXIT_FAILURE);
            }

            clientQueue.push(clientSocketFD);
            std::cout << "New client connected, address: " << inet_ntoa(clientAddress.sin_addr) << ":" << ntohs(clientAddress.sin_port) << ", socketFD: " << clientSocketFD << std::endl;

            for (size_t socketID = 0; socketID < MAX_CLIENTS; socketID++)
            {
                if (clientSocketList[socketID] == 0)
                {
                    clientSocketList[socketID] = clientSocketFD;
                    std::cout << "Adding to list of sockets at " << socketID << " index" << std::endl;

                    break;
                }
            }
        }

        for (size_t socketID = 0; socketID < MAX_CLIENTS; socketID++)
        {
            clientSocketFD = clientSocketList[socketID];

            // Checking wether server received from the correct/current response client/tag
            if (currentClientSocketFD == clientSocketFD)
            {
                if (FD_ISSET(clientSocketFD, &readFDS))
                {
                    nbytes = read(clientSocketFD, buffer, sizeof(buffer));

                    currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
                    timestamp = currentTime.count();
                    responseTime = std::chrono::high_resolution_clock::now();

                    // std::cout << "Read request \n";
                    std::string request(buffer, nbytes);
                    std::cout << "Received distance " << request << " from client: " << clientSocketFD << std::endl;

                    // Check whether recording has been paused
                    if (!sharedData.isRecordingPaused())
                    {
                        timestampFile << dataIndex << " " << timestamp << " " << request;
                        const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(responseTime - requestTime);
                        timestampFile << "Request time: " << std::chrono::duration_cast<std::chrono::milliseconds>(requestTime.time_since_epoch()).count() << "\n";
                        timestampFile << "Response time: " << std::chrono::duration_cast<std::chrono::milliseconds>(responseTime.time_since_epoch()).count() << "\n";
                        timestampFile << "Overall time of the request (response time - request time): " << duration.count() << "\n"
                                      << std::endl;

                        // if (debugMode)
                        // {
                        //     std::cout << "Data " << dataIndex << " is recorded" << std::endl;
                        // }
                        dataIndex++;
                    }

                    std::string responseToTag = "7\n";
                    write(clientSocketFD, responseToTag.c_str(), responseToTag.length());

                    clientQueue.push(clientSocketFD);
                    isBusy = false;
                    currentClientSocketFD = -1;
                }

                // Disconnect tag if it was disconnected during request
                if (isBusy && !FD_ISSET(serverSocketFD, &readFDS))
                {
                    isBusy = false;
                    std::cout << "Client " << clientSocketFD << " was disconnected!" << std::endl;
                    close(currentClientSocketFD);
                    FD_CLR(currentClientSocketFD, &readFDS);
                    clientSocketList[socketID] = 0;
                    currentClientSocketFD = -1;
                    // break;
                }

                break;
            }
        }

        // std::cout << "isBusy: " << isBusy << std::endl;
        if (!clientQueue.empty() && !isBusy)
        {
            clientSocketFD = clientQueue.front();
            clientQueue.pop();

            std::string request = "1\n";
            requestTime = std::chrono::high_resolution_clock::now();
            write(clientSocketFD, request.c_str(), request.length());
            isBusy = true;
            currentClientSocketFD = clientSocketFD;

            // std::cout << "Sent request for distance to the client with socketID: " << clientSocketFD << std::endl;
        }
    }
}

// int main()
// {

//     Server::runServer();

//     return 0;
// }
