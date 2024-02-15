#ifndef SHAREDDATA_H
#define SHAREDDATA_H

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
        // std::lock_guard<std::mutex> lock(mtx);
        return isPause;
    }

    void setTerminationFlag()
    {
        std::lock_guard<std::mutex> lock(mtx);
        isTermination = true;
    }

    bool terminationFlag()
    {
        // std::lock_guard<std::mutex> lock(mtx);
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