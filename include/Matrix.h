#pragma once

#include <vector>
#include <stdexcept>
#include <memory>

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

    // ==========================================
    // 1. 复合赋值运算符（成员函数，返回 Matrix&）
    // ==========================================
    Matrix& operator+=(float scalar);
    Matrix& operator-=(float scalar);
    // 标量复合运算
    Matrix& operator*=(float scalar);
    Matrix& operator/=(float scalar);
    // 矩阵乘法复合运算（注意：内部形状可能会改变）
    Matrix& operator*=(const Matrix& rhs);
    // ==========================================
    // 2. 算术运算符（非成员 / 友元函数，按值返回 Matrix）
    // ==========================================
    // 矩阵与矩阵加减
    friend Matrix operator+(const Matrix& lhs, const Matrix& rhs);
    friend Matrix operator-(const Matrix& lhs, const Matrix& rhs);
    // 矩阵与矩阵乘法（线性代数乘法）
    friend Matrix operator*(const Matrix& lhs, const Matrix& rhs);
    // 矩阵与标量乘法（必须支持双向交换律）
    friend Matrix operator*(const Matrix& mat, double scalar);
    friend Matrix operator*(double scalar, const Matrix& mat);
    // 矩阵与标量除法
    friend Matrix operator/(const Matrix& mat, double scalar);
    

private:
    int rows_;
    int cols_;
    std::vector<float> data_;

    // 内部边界检查辅助函数
    void check_bounds(int r, int c) const;
};


class MatrixManual{
public:
    explicit MatrixManual(int rows, int cols);
    ~MatrixManual();

    MatrixManual(const MatrixManual& other);
    MatrixManual& operator=(const MatrixManual& other);

private:
    int rows_;
    int cols_;
    float* data_;
};

class MatrixUnique{
public:
    explicit MatrixUnique(int rows, int cols);
    ~MatrixUnique();

    MatrixUnique(const MatrixUnique& other) = delete; // 禁止拷贝构造
    MatrixUnique& operator=(const MatrixUnique& other) = delete; // 禁止拷贝赋值

    MatrixUnique(MatrixUnique&& other) noexcept; // 移动构造
    MatrixUnique& operator=(MatrixUnique&& other) noexcept; // 移动赋值

private:
    int rows_;
    int cols_;
    std::unique_ptr<float[]> data_;
};
