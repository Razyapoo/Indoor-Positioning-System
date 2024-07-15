#ifndef SHAREDDATA_H
#define SHAREDDATA_H

// Allows to synchronize threads. For example when server do not receive the data, video is not recording
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