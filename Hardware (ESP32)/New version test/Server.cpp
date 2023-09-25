#include "Server.hpp"

int Server::serverSocketFD, Server::clientSocketFD;
struct sockaddr_in Server::serverAddress, Server::clientAddress;
socklen_t Server::clientLength;
char Server::buffer[1024];
std::queue<int> Server::clientQueue;
fd_set Server::readFDS, Server::tmpFDS;
size_t Server::dataIndex = 1;
std::chrono::system_clock::time_point Server::currentTime;
std::time_t Server::timestamp;
bool Server::isBusy = false;
struct timeval Server::timeout;

// DEBUG
bool Server::debugMode = true;

void Server::printFDSet(fd_set *set)
{
    std::cout << "fd_set {" << std::endl;
    for (size_t i = 0; i < FD_SETSIZE; i++)
    {
        if (FD_ISSET(i, set))
        {
            std::cout << i << std::endl;
        }
    }

    std::cout << "}" << std::endl;
}

void Server::showQueue(std::queue<int> queue)
{
    std::queue<int> tempQueue = queue;
    while (!tempQueue.empty())
    {
        std::cout << tempQueue.front() << "\t";
        tempQueue.pop();
    }
    std::cout << std::endl;
}

void Server::runServer()
{
    using namespace std::literals::chrono_literals;

    serverSocketFD = socket(AF_INET, SOCK_STREAM, 0);

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(30001);
    bind(serverSocketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

    listen(serverSocketFD, 5);

    FD_ZERO(&readFDS);
    FD_SET(serverSocketFD, &readFDS);

    std::ofstream timestampFile("timestamp_ESP32.txt");
    if (!timestampFile.is_open())
        throw std::runtime_error("Failed to open timestamp_ESP32.txt file");

    while (true)
    {
        timeout.tv_sec = 10;
        timeout.tv_usec = 0;

        if (debugMode)
        {
            std::cout << "before select" << std::endl;
            printFDSet(&readFDS);
        }

        tmpFDS = readFDS;
        int result = select(FD_SETSIZE, &tmpFDS, nullptr, nullptr, &timeout);

        if (debugMode)
        {
            std::cout << "after select" << std::endl;
            printFDSet(&tmpFDS);
        }

        std::cout << "Value of the result" << result << std::endl;
        if (result)
        {
            for (size_t socketID = 0; socketID < FD_SETSIZE; ++socketID)
            {
                if (FD_ISSET(socketID, &tmpFDS))
                {
                    if (serverSocketFD == socketID)
                    {
                        clientLength = sizeof(clientAddress);
                        clientSocketFD = accept(serverSocketFD, (struct sockaddr *)&clientAddress, &clientLength);
                        clientQueue.push(clientSocketFD);
                        std::cout << "New client connected, address: " << inet_ntoa(clientAddress.sin_addr) << ":" << ntohs(clientAddress.sin_port) << ", socketID: " << clientSocketFD << std::endl;
                        FD_SET(clientSocketFD, &readFDS);
                    }
                    else
                    {
                        int nbytes = read(socketID, buffer, sizeof(buffer));
                        if (nbytes < 1)
                        {
                            close(socketID);
                            FD_CLR(socketID, &readFDS);
                            std::cout << "Client with socket ID: " << socketID << " has been disconnected" << std::endl;
                        }
                        else
                        {
                            std::string request(buffer, nbytes);
                            std::cout << "Received distance " << request << " from client: " << socketID << std::endl;

                            // currentTime = std::chrono::system_clock::now();
                            // currentTime = std::chrono::high_resolution_clock::now();
                            // timestamp = std::chrono::system_clock::to_time_t(currentTime);
                            // timestampFile << dataIndex << " " << timestamp << " " << request << std::endl;

                            // if (debugMode) std::cout << "Data " << dataIndex << " is recorded" << std::endl;
                            // dataIndex++;

                            std::string responseToTag = "7\n";
                            write(socketID, responseToTag.c_str(), responseToTag.length());

                            std::cout << "queue before add" << std::endl;
                            showQueue(clientQueue);
                            clientQueue.push(socketID);
                            std::cout << "queue after add" << std::endl;
                            showQueue(clientQueue);
                            std::cout << "sleep for 2 sec" << std::endl;
                            std::this_thread::sleep_for(1s);
                            isBusy = false;
                        }
                    }
                }
            }
        }
        else
        {
            continue;
        }

        if (!clientQueue.empty() && !isBusy)
        {
            std::cout << "queue before select" << std::endl;
            showQueue(clientQueue);

            clientSocketFD = clientQueue.front();
            clientQueue.pop();
            std::cout << "queue after select" << std::endl;
            showQueue(clientQueue);

            std::string request = "1\n";
            write(clientSocketFD, request.c_str(), request.length());

            if (debugMode)
                std::cout << "Sent request for distance to the client with socketID: " << clientSocketFD << std::endl;
            isBusy = true;
        }
    }
    // timestampFile.close();
}

int main()
{

    Server::runServer();

    return 0;
}