#include "Server.hpp"
#include "VideoManager.hpp"
#include "SharedData.hpp"
#include <iostream>
#include <thread>

SharedData sharedData;

void startCamera()
{
    StereoCamera::initStereoCamera(2, 4);
    VideoManager::videoRecorder();
}

void startServer()
{
    Server::runServer();
}

void startActivityWatchdog()
{
    Server::checkForActive();
}

int main()
{
    // std::thread camera_thread(startCamera);
    std::thread server_thread(startServer);
    std::thread watchdog_thread(startActivityWatchdog);

    // camera_thread.join();
    server_thread.join();
    watchdog_thread.join();

    return 0;
}