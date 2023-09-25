#include "Server.hpp"

int Server::serverSocketFD, Server::clientSocketFD;
int Server::opt = 1;

fd_set Server::readFDS, Server::tmpFDS;

char Server::buffer[1024];
bool Server::isBusy = false;
size_t Server::dataIndex = 1;

struct sockaddr_in Server::serverAddress, Server::clientAddress;
struct timeval Server::timeout;

socklen_t Server::clientLength;
std::queue<int> Server::clientQueue;
std::chrono::milliseconds Server::currentTime;
std::time_t Server::timestamp;

// DEBUG
bool Server::debugMode = true;
// using namespace std::literals::chrono_literals;

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

    FD_ZERO(&readFDS);
    FD_SET(serverSocketFD, &readFDS);

    std::ofstream timestampFile("timestamp_ESP32.txt");
    if (!timestampFile.is_open())
        throw std::runtime_error("Failed to open timestamp_ESP32.txt file");

    while (true)
    {
        timeout.tv_sec = 3;
        timeout.tv_usec = 0;

        if (debugMode)
        {
            std::cout << "before select" << std::endl;
            printFDSet(&readFDS);
        }

        tmpFDS = readFDS;
        select(FD_SETSIZE, &tmpFDS, nullptr, nullptr, &timeout);

        if (debugMode)
        {
            std::cout << "after select" << std::endl;
            printFDSet(&tmpFDS);
        }

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

                        currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
                        timestamp = currentTime.count();
                        timestampFile << dataIndex << " " << timestamp << " " << request << std::endl;

                        if (debugMode)
                            std::cout << "Data " << dataIndex << " is recorded" << std::endl;
                        dataIndex++;

                        std::string responseToTag = "7\n";
                        write(socketID, responseToTag.c_str(), responseToTag.length());
                        // std::this_thread::sleep_for(1s);

                        clientQueue.push(socketID);
                        isBusy = false;
                    }
                }
            }
        }

        if (!clientQueue.empty() && !isBusy)
        {
            clientSocketFD = clientQueue.front();
            clientQueue.pop();

            std::string request = "1\n";
            write(clientSocketFD, request.c_str(), request.length());
            isBusy = true;

            if (debugMode)
                std::cout << "Sent request for distance to the client with socketID: " << clientSocketFD << std::endl;
        }
    }
    timestampFile.close();
}

// int main()
// {

//     Server::runServer();

//     return 0;
// }