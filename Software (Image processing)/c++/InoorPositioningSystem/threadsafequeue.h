#ifndef THREADSAFEQUEUE_H
#define THREADSAFEQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <QImage>

#include "structures.h"

// Thread-safe queue for producer-consumer problem
// https://unscriptedcoding.medium.com/concurrency-and-multithreading-40b264ecc9ae


class ThreadSafeQueue
{
public:
    ThreadSafeQueue(size_t capacity);
    ThreadSafeQueue();
    ~ThreadSafeQueue();

    void enqueue(UWBVideoData&& data);
    // void enqueue(int position, QImage qImage);
    // void search(const int& position, UWBVideoData& uwbVideoData);
    bool dequeue(UWBVideoData& data);
    bool isEmpty();
    void notify_all();
    void clear();
    size_t size();


private:
    std::queue<UWBVideoData> buffer;
    std::mutex mtx;
    std::condition_variable cvar;
    size_t capacity;
};

#endif // THREADSAFEQUEUE_H
