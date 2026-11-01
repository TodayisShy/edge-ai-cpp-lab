// 在 Linux / WSL 上运行

#include "SharedTensorMap.h"

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <chrono>
#include <thread>
#include <cstring>
#include <cerrno>

int main() {
    // 1. open(kShmPath, O_RDWR)   // 文件不存在就 perror 退出，先起 Producer
    // 2. mmap(..., PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)
    // 3. 循环等 t->ready == 1，每次 sleep_for(10ms)，设个超时以免死等
    // 4. 打印本进程的指针 t（可以和 Producer 的不同）
    // 5. 校验 numel、data[i] == i * 0.1f
    // 6. munmap / close
    int fd = open(kShmPath, O_RDWR);
    if (fd == -1) {
        std::cerr << "open failed: " << strerror(errno) << std::endl;
        return 1;
    }
    void* ptr = mmap(nullptr, shm_bytes(), PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (ptr == MAP_FAILED) {
        std::cerr << "mmap failed: " << strerror(errno) << std::endl;
        return 1;
    }
    auto start_time = std::chrono::steady_clock::now();
    SharedTensor* t = static_cast<SharedTensor*>(ptr);
    while (true){
        if (t->ready == 1) {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if (std::chrono::steady_clock::now() - start_time > std::chrono::seconds(5)) {
            std::cerr << "timeout" << std::endl;
            return 1;
        }
    }
    std::cout << "t: " << t << ", data[15]: " << t->data[15] << ", data[14]: " << t->data[14] << std::endl;
    for (int i = 0; i < kNumel; i++) {
        if (t->data[i] != i * 0.1f) {
            std::cerr << "data[" << i << "] is not correct" << std::endl;
            return 1;
        }
    }
    munmap(ptr, shm_bytes());
    close(fd);
    return 0;
}