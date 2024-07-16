#ifndef SHAREDDATA_H
#define SHAREDDATA_H

// Allows to synchronize threads. 

/*********************************************** Shared Data **********************************************************
 * This is an auxiliary structure, which helps in communcation between the Server, Video Manager and Activity Watchdog
 * For example, it ensures safe termination of the Server_Multithreaded (when "s" is pressed): 
 *  - safe shutdown of the (UWB) Server
 *  - safe releasing cv::VideoCapture webcam
 *  - pause recording of both data streams (when "p" is pressed in cv::imshow window)
***********************************************************************************************************************/

#include <mutex>
#include <chrono>
#include <ctime>

class SharedData
{
public:
    SharedData() : isPause(false), isTermination(false) {}

    void pauseRecording()
    {
        std::lock_guard<std::mutex> lock(mtx);
        isPause = true;
    }

    void startRecording()
    {
        std::lock_guard<std::mutex> lock(mtx);
        isPause = false;
    }

    bool isRecordingPaused()
    {
        return isPause;
    }

    void setTerminationFlag()
    {
        std::lock_guard<std::mutex> lock(mtx);
        isTermination = true;
    }

    bool terminationFlag()
    {
        return isTermination;
    }

    void updateLastActivityTimePoint(std::chrono::high_resolution_clock::time_point timePoint)
    {
        std::lock_guard<std::mutex> lock(mtx);
        lastActivityTimePoint = timePoint;
    }

    const std::chrono::high_resolution_clock::time_point &getLastActivityTimePoint()
    {
        return lastActivityTimePoint;
    }

private:
    bool isPause, isTermination;
    std::chrono::high_resolution_clock::time_point lastActivityTimePoint;

    std::mutex mtx;
};

#endif