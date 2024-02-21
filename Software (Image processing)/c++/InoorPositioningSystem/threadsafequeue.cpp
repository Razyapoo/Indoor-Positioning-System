#include "threadsafequeue.h"

ThreadSafeQueue::ThreadSafeQueue(size_t capacity): capacity(capacity) {}
ThreadSafeQueue::~ThreadSafeQueue() {}

// void ThreadSafeQueue::enqueue(int position, QImage qImage) {
//     std::unique_lock<std::mutex> lock(mtx);
//     // Adding capacity to controll the size of queue in case of hours of video
//     cvar.wait(lock, [this] {return buffer.size() < capacity; });
//     UWBVideoData data;
//     data.videoData.qImage = std::move(qImage);
//     data.videoData.timestamp = -1;
//     data.uwbData.id = -1; // allows to check later whether uwb data is set
//     buffer.push(data);
//     cvar.notify_all();
// }


void ThreadSafeQueue::enqueue(UWBVideoData&& data) {
    std::unique_lock<std::mutex> lock(mtx);
    // Adding capacity to controll the size of queue in case of hours of video
    cvar.wait(lock, [this] {return buffer.size() < capacity; });
    buffer.push(std::move(data));
    cvar.notify_all();
}

bool ThreadSafeQueue::dequeue(UWBVideoData& data) {
    std::unique_lock<std::mutex> lock(mtx);
    cvar.wait(lock, [this]{return !buffer.empty();});
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
}

// void ThreadSafeQueue::search(const int& position, UWBVideoData& uwbVideoData) {


// }
