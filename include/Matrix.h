#pragma once

#include <vector>

class Matrix {
public:
    // 构造函数：声明在头文件，实现在 cpp 文件
    explicit Matrix(int rows = 0, int cols = 0);

    // 简单 Getter：保留在头文件内联展开以获得最优调用性能
    int rows() const noexcept { return rows_; }
    int cols() const noexcept { return cols_; }

    // 元素读写操作符：包含越界检查等逻辑，只在头文件做声明
    float& operator()(int r, int c);
    const float& operator()(int r, int c) const;

private:
    int rows_;
    int cols_;
    std::vector<float> data_;

    // 内部边界检查辅助函数
    void check_bounds(int r, int c) const;
};