#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

class ThreadSafeQueue {
public:
    explicit ThreadSafeQueue(int capacity = 10){capacity_ = capacity;};
    void push(int item);
    int pop();
    bool empty() const;
    int capacity() const noexcept {return capacity_;};
    void set_capacity(int capacity);
    bool full() const;

private:
    mutable std::mutex mtx_;
    std::condition_variable cv_;
    std::condition_variable cv_full_;
    std::queue<int> q_;
    int capacity_;
};