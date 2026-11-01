// 在 Linux / WSL 上运行

#include "SharedTensorMap.h"

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <cerrno>
#include <cstring>

int main() {
    // 1. open(kShmPath, O_RDWR | O_CREAT | O_TRUNC, 0600)
    // 2. ftruncate(fd, shm_bytes())     // 不拉长，后面写会 SIGBUS
    // 3. mmap(nullptr, shm_bytes(), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)
    //    失败比较的是 MAP_FAILED，不是 nullptr
    // 4. 得到 SharedTensor* t
    //    t->numel = kNumel
    //    data[i] = i * 0.1f
    //    最后 t->ready = 1
    // 5. 打印本进程的指针 t，以及 data[0], data[1]
    // 6. sleep 几秒，等 Consumer 来读
    // 7. munmap / close   （今天先不要 unlink，方便反复跑 Consumer）
    int fd = open(kShmPath, O_RDWR | O_CREAT | O_TRUNC, 0600);
    if (fd == -1) {
        std::cerr << "open failed: " << strerror(errno) << std::endl;
        return 1;
    }
    if (ftruncate(fd, shm_bytes()) == -1) {
        std::cerr << "ftruncate failed: " << strerror(errno) << std::endl;
        return 1;
    }
    void* ptr = mmap(nullptr, shm_bytes(), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        std::cerr << "mmap failed: " << strerror(errno) << std::endl;
        return 1;
    }
    SharedTensor* t = static_cast<SharedTensor*>(ptr);
    t->numel = kNumel;
    for (int i = 0; i < kNumel; i++) {
        t->data[i] = i * 0.1f;
    }
    t->ready = 1;
    std::cout << "t: " << t << ", data[0]: " << t->data[0] << ", data[1]: " << t->data[1] << std::endl;
    sleep(3);
    munmap(ptr, shm_bytes());
    ptr = nullptr;
    close(fd);
    return 0;
}