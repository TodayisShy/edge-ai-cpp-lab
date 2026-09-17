#pragma once

#include <vector>
#include <stdexcept>
#include <memory>


class Tensor {
public:
    explicit Tensor(std::vector<int> shape);

    size_t numel()const noexcept;
    const std::vector<int>& shape() const noexcept;
    float* data() noexcept;
    const float* data() const noexcept;
    void reshape(const std::vector<int>& new_shape);
    
private:
    std::vector<int> shape_;
    std::vector<float> data_;

    // 内部边界检查辅助函数
    void check_bounds(int r, int c) const;
};