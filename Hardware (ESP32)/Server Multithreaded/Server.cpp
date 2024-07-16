#include "Server.hpp"

int Server::serverSocketFD, Server::clientSocketFD, Server::currentClientSocketFD;
int Server::maxSocketFD;
int Server::opt = 1;
int Server::activity;
fd_set Server::readFDS, Server::tmpFDS;
char Server::buffer[1024];
const size_t Server::MAX_CLIENTS = 5;
size_t Server::nbytes;
std::vector<int> Server::clientSocketList(MAX_CLIENTS, 0);
struct sockaddr_in Server::serverAddress, Server::clientAddress;
socklen_t Server::clientAddrLength;
struct timeval Server::timeout;

std::queue<int> Server::clientQueue;
std::chrono::milliseconds Server::currentTime;
std::chrono::time_point<std::chrono::high_resolution_clock> Server::requestTime, Server::responseTime;
std::time_t Server::timestamp;
bool Server::isBusy = false;
size_t Server::dataIndex = 1;

int Server::statusImageHeight = 640;
int Server::statusImageWidth = 720;

extern SharedData sharedData; // extern shared variable that is used for communication between threads 

bool Server::debugMode = true; // DEBUG

// Helping structure, showing which file descriptors are already set
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

/* Visual check for active connection with tags
*  Colors (window background):
*       - GREEN: active 
*       - RED: blocked
*/
void Server::checkForActive()
{
    cv::Mat statusImage;
    std::chrono::high_resolution_clock::time_point currentTimePoint;
    std::chrono::duration<double> noResponseDuration;

    // Until program is stopped (Video Manager requested to stop)
    while (!sharedData.terminationFlag()) 
    {
        currentTimePoint = std::chrono::high_resolution_clock::now();
        noResponseDuration = std::chrono::duration_cast<std::chrono::duration<double>>(currentTimePoint - sharedData.getLastActivityTimePoint());

        // check for 1 sec
        if (noResponseDuration.count() > 1)
        {
            statusImage = cv::Mat(statusImageHeight, statusImageWidth, CV_8UC3, cv::Scalar(0, 0, 255));
        }
        else
        {
            statusImage = cv::Mat(statusImageHeight, statusImageWidth, CV_8UC3, cv::Scalar(0, 255, 0));
        }

        cv::imshow("Activeness", statusImage);
        cv::waitKey(1);
    }
}

void Server::runServer()
{
    cv::namedWindow("Activeness", 1);
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

    clientAddrLength = sizeof(clientAddress);
    for (size_t socketID = 0; socketID < MAX_CLIENTS; socketID++)
    {
        clientSocketList[socketID] = 0;
    }

    // Try to open UWB_timestamps.txt 
    std::ofstream timestampFile("UWB_timestamps.txt");
    timestampFile.clear();
    if (!timestampFile.is_open())
        throw std::runtime_error("Failed to open UWB_timestamps.txt file");
    
    while (true)
    {
        // If stop was requested by video manager
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

        // Check activity on socket. 
        // Timeout is needed to handle new tag connections to the server
        // so the server is not blocked waiting for response from the active tag, if it takes long time 
        activity = select(maxSocketFD + 1, &readFDS, NULL, NULL, &timeout);

        if ((activity < 0) && (errno != EINTR))
        {
            perror("Error during select");
        }

        std::chrono::high_resolution_clock::time_point currentTimePoint = std::chrono::high_resolution_clock::now();
        sharedData.updateLastActivityTimePoint(currentTimePoint); // some activity was recorded

        // Handling new (unknown) connections to the server
        if (FD_ISSET(serverSocketFD, &readFDS))
        {
            if ((clientSocketFD = accept(serverSocketFD, (struct sockaddr *)&clientAddress, (socklen_t *)&clientAddrLength)) < 0)
            {
                perror("Accept error!");
                exit(EXIT_FAILURE);
            }

            // Add newly discovered tag to the queue for further communication
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

        // Request from known tag
        for (size_t socketID = 0; socketID < MAX_CLIENTS; socketID++)
        {
            clientSocketFD = clientSocketList[socketID]; // select correct socket file descriptor

            // Checking wether server has received response from the correct (active) tag
            if (currentClientSocketFD == clientSocketFD)
            {   
                // Received request from the tag. Tag has sent the measured distances!!
                if (FD_ISSET(clientSocketFD, &readFDS))
                {
                    nbytes = read(clientSocketFD, buffer, sizeof(buffer)); // read message constaining the distances

                    // Record time of the recept
                    currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
                    timestamp = currentTime.count();
                    responseTime = std::chrono::high_resolution_clock::now();

                    std::string request(buffer, nbytes);
                    std::cout << "Received distance " << request << " from client: " << clientSocketFD << std::endl;

                    // Check if recording is paused
                    if (!sharedData.isRecordingPaused())
                    {
                        // write measurements and timestamps to the output file (UWB_timestamps.txt)
                        timestampFile << dataIndex << " " << timestamp << " " << request;
                        const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(responseTime - requestTime);
                        timestampFile << "Request time: " << std::chrono::duration_cast<std::chrono::milliseconds>(requestTime.time_since_epoch()).count() << "\n";
                        timestampFile << "Response time: " << std::chrono::duration_cast<std::chrono::milliseconds>(responseTime.time_since_epoch()).count() << "\n";
                        timestampFile << "Overall time of the request (response time - request time): " << duration.count() << "\n"
                                      << std::endl;

                        dataIndex++;
                    }

                    // Response with ACK - show successful receipt
                    std::string responseToTag = "7\n";
                    write(clientSocketFD, responseToTag.c_str(), responseToTag.length());

                    // Remember the tag for new iterations
                    clientQueue.push(clientSocketFD);
                    isBusy = false;
                    currentClientSocketFD = -1;
                }

                // Disconnecting the tag if it is disconnected while communicating with the server
                if (isBusy && !FD_ISSET(serverSocketFD, &readFDS))
                {
                    isBusy = false;
                    std::cout << "Client " << clientSocketFD << " was disconnected!" << std::endl;
                    close(currentClientSocketFD);
                    FD_CLR(currentClientSocketFD, &readFDS);
                    clientSocketList[socketID] = 0;
                    currentClientSocketFD = -1;
                }

                break;
            }
        }

        // Handling old connections
        // selecting next tag to initiate communication with anchors
        if (!clientQueue.empty() && !isBusy)
        {
            clientSocketFD = clientQueue.front();  // select next tag for communication
            clientQueue.pop();

            std::string request = "1\n"; // request initiation indicator: "Mesure!"

            /* Server records the time when it sent the request "Measure!" to a tag;
            * when the tag responses with distance measurements, the server records the 
            * receiption time and calculates the difference (response - request time)
            * this gives a time spent measuring the distances by the tag. 
            * Best and most common time of mesuring is 100ms (fast) !!!
            * this means that tags are able to calculate people positions at a frequency of 10Hz each
            */
            requestTime = std::chrono::high_resolution_clock::now();
            write(clientSocketFD, request.c_str(), request.length());
            isBusy = true;
            currentClientSocketFD = clientSocketFD;
        }
    }
}