#ifndef SHAREDDATA_H
#define SHAREDDATA_H

#include <mutex>

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
        std::lock_guard<std::mutex> lock(mtx);
        return isPause;
    }

    void setTerminationFlag()
    {
        std::lock_guard<std::mutex> lock(mtx);
        isTermination = true;
    }

    bool terminationFlag()
    {
        std::lock_guard<std::mutex> lock(mtx);
        return isTermination;
    }

private:
    bool isPause;
    bool isTermination;
    std::mutex mtx;
};

#endif