#ifndef THREADSAFEQUEUE_H
#define THREADSAFEQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

#include "structures.h"

/**************************************** Thread Safe Queue *************************************************************
 * This is a thread safe Video + UWB data container
 * All threads are working with this structure
 * Pipeline: VideoProcessr (read video frame) -> DataProcessor (sync with UWB data) -> VideoPlayer (show synced data)
 * IMPORTANT: capacity of the queue should be set low (in constructor of VideModel), causes high load on machine
*************************************************************************************************************************/

class ThreadSafeQueue
{
public:
    ThreadSafeQueue(size_t capacity);
    ~ThreadSafeQueue();

    void enqueue(UWBVideoData&& data);
    bool dequeue(UWBVideoData& data);
    bool isEmpty();
    void notify_all();
    void interruptionRequest(); // in the case when immediate interruption is required
    void clear();
    size_t size();


private:
    std::queue<UWBVideoData> buffer;
    std::mutex mtx;
    std::condition_variable cvar;
    size_t capacity;
    std::atomic<bool> isInterruptionRequested, isStopRequested; // safety watchdogs; otherwise might be deadlock
};

#endif // THREADSAFEQUEUE_H
