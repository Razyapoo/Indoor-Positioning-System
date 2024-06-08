#include "threadsafequeue.h"

ThreadSafeQueue::ThreadSafeQueue(size_t capacity): capacity(capacity), isInterruptionRequested(false), isStopRequested(false) {}
ThreadSafeQueue::~ThreadSafeQueue() {
    isStopRequested = true;
    clear();
}


void ThreadSafeQueue::enqueue(UWBVideoData&& data) {
    std::unique_lock<std::mutex> lock(mtx);
    // Adding capacity to controll the size of queue in case of hours of video
    cvar.wait(lock, [this] {return buffer.size() < capacity || isInterruptionRequested || isStopRequested; });
    if (isInterruptionRequested || isStopRequested) {
        isInterruptionRequested = false;
        return;
    }
    buffer.push(std::move(data));
    cvar.notify_all();
}

bool ThreadSafeQueue::dequeue(UWBVideoData& data) {
    std::unique_lock<std::mutex> lock(mtx);
    cvar.wait(lock, [this]{return !buffer.empty() || isStopRequested; });
    if (isStopRequested) return false;
    data = std::move(buffer.front());
    buffer.pop();
    cvar.notify_all();
    return true;
}

void ThreadSafeQueue::notify_all() {
    cvar.notify_all();
}

bool ThreadSafeQueue::isEmpty() {
    std::lock_guard<std::mutex> lock(mtx);
    return buffer.empty();
}

void ThreadSafeQueue::clear() {
    std::lock_guard<std::mutex> lock(mtx);
    while (!buffer.empty()) {
        buffer.pop();
    }
    cvar.notify_all();
}

void ThreadSafeQueue::interruptionRequest() {
    std::lock_guard<std::mutex> lock(mtx);
    isInterruptionRequested = true;
    cvar.notify_all();
}

