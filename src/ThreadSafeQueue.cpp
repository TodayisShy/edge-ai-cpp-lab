#include "ThreadSafeQueue.h"

void ThreadSafeQueue::push(int item) {
    // TODO: 拿锁 → 放进 q_ → 敲门
    std::unique_lock<std::mutex> lock(mtx_);
    cv_full_.wait(lock, [this] { return q_.size() < capacity_; });
    q_.push(item);
    cv_.notify_one();
}

int ThreadSafeQueue::pop() {
    // TODO: unique_lock → 空就 wait → 取出队头 → return
    std::unique_lock<std::mutex> lock(mtx_);
    cv_.wait(lock, [this] { return !q_.empty(); });
    int item = q_.front();
    q_.pop();
    cv_full_.notify_one();
    return item;
}

bool ThreadSafeQueue::empty() const {
    // TODO: 拿锁 → return q_.empty()
    std::unique_lock<std::mutex> lock(mtx_);
    return q_.empty();
}

void ThreadSafeQueue::set_capacity(int capacity){
    capacity_ = capacity;
};

bool ThreadSafeQueue::full() const{
    std::unique_lock<std::mutex> lock(mtx_);
    return q_.size() >= capacity_;
}