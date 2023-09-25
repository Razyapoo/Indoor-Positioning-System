#include "Server.hpp"
#include "VideoManager.hpp"
#include <iostream>
#include <thread>

void startCamera()
{
    StereoCamera::initStereoCamera(2, 4);
    VideoManager::videoRecorder();
}

void startServer()
{
    Server::runServer();
}

int main()
{
    std::thread camera_thread(startCamera);
    std::thread server_thread(startServer);

    camera_thread.join();
    // server_thread.join();

    return 0;
}