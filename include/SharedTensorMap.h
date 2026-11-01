#pragma once

#include <cstddef>

inline constexpr const char* kShmPath = "/tmp/edge_ai_tensor.shm";
inline constexpr int kNumel = 16;

struct SharedTensor {
    int ready;          // 0 = 还在写, 1 = 可以读
    int numel;
    float data[kNumel];
};

inline std::size_t shm_bytes() {
    return sizeof(SharedTensor);
}