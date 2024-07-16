/*********************************************** Server Multithreaded *****************************************************************
 * Initiates simultaneous work of (in dedicated threads):
 *      - Video Manager
 *      - (UWB) Server
 *      - Activity watchdog separately
 * 
 * !! These data are not yet synchornized; synchronization is performed later in Indoor Positioning System (GUI)
 * 
 * Responsibilities:
 *      - Video processing: handles video recording
 *      - Server: communicates with tags and collects distance measurements from them
 *      - Activity watchdog: monitors tag responses and detects if communication between anchor and tag is blocked
***************************************************************************************************************************************/

#include "Server.h"
#include "VideoManager.h"
#include "SharedData.h"
#include "Camera.h"
#include <iostream>
#include <thread>

SharedData sharedData;
 
void startCamera()
{
    Camera::initCamera(2);
    VideoManager::runVideoRecorder();
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
    std::thread camera_thread(startCamera);
    std::thread server_thread(startServer);
    std::thread watchdog_thread(startActivityWatchdog);

    camera_thread.join();
    server_thread.join();
    watchdog_thread.join();

    return 0;
}