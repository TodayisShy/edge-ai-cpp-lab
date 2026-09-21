#pragma once

#include <vector>
#include <stdexcept>
#include <memory>
#include <string>

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


class TensorBuffer {
public:
    // 构造函数使用 const reference
    explicit TensorBuffer(const std::vector<int>& shape, const std::string& name = "");

    // Rule of 5: 显式管理所有权与生命周期
    TensorBuffer(const TensorBuffer& other);
    TensorBuffer& operator=(const TensorBuffer& other);
    TensorBuffer(TensorBuffer&& other) noexcept;
    TensorBuffer& operator=(TensorBuffer&& other) noexcept;
    ~TensorBuffer();

    // 观察器 (Inspectors)
    size_t numel() const noexcept;
    const std::vector<int>& shape() const noexcept {return shape_;} 
    const std::string& name() const noexcept {return name_;} 
    // 原始缓冲区访问
    float* data() noexcept;
    const float* data() const noexcept;

    // 元素安全访问（带边界检查）
    float& at(size_t index);
    const float& at(size_t index) const;

    // 数据与形状修改
    void reshape(const std::vector<int>& new_shape);
    void fill(float value) noexcept;

private:
    // 调整声明顺序：先元数据，后物理内存！
    std::string name_;
    std::vector<int> shape_;
    size_t numel_ = 0;
    std::unique_ptr<float[]> data_;

    void check_bounds(size_t index) const;
};