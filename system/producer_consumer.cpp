#include "ThreadSafeQueue.h"

#include <iostream>
#include <thread>
#include <string>

void producer(ThreadSafeQueue& q);
void consumer(ThreadSafeQueue& q);

int main() {
    // TODO: 建 Queue，开两个线程，join
    ThreadSafeQueue q(5);
    std::thread producer_thread(producer, std::ref(q));
    std::thread consumer_thread(consumer, std::ref(q));
    producer_thread.join();
    consumer_thread.join();
    return 0;
}

void producer(ThreadSafeQueue& q) {
    // TODO: push 1..10，每放一个打印 produced
    for (int i = 1; i <= 7; ++i) {
        std::cout << "[producer]: " + std::to_string(i) + "\n";
        q.push(i);
    }
    std::cout << "[producer]: done\n";
    q.push(-1);
}

void consumer(ThreadSafeQueue& q) {
    // TODO: pop 10 次，每拿一个打印 consumed
    for (int i = 1; i <= 10; ++i) {
        int item = q.pop();
        std::cout << "[consumer]: " + std::to_string(item) + "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (item == -1) {
            break;
        }
    }
    std::cout << "[consumer]: done\n";
}