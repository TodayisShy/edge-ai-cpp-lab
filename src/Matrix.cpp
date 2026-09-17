#include <stdexcept>
#include "Matrix.h"

using namespace std;

Matrix::Matrix(int rows, int cols) : rows_(rows), cols_(cols), data_(rows * cols, 0.0f) {
    // 构造函数实现：初始化矩阵大小和数据
    if(rows <= 0 || cols <= 0){
        throw std::invalid_argument("Matrix dimensions must be positive");
    }
}

float& Matrix::operator()(int r, int c){
    check_bounds(r,c);
    return data_[static_cast<size_t>(r)*cols_ + c];
}

const float& Matrix::operator()(int r, int c) const {
    check_bounds(r,c);
    return data_[static_cast<size_t>(r)*cols_ + c];
}

// ==========================================
// 1. 复合赋值运算符（成员函数，返回 Matrix&）
// ==========================================
Matrix& Matrix::operator+=(float scalar){
    for(auto& val : data_){
        val += scalar;
    }
    return *this;
}
Matrix& Matrix::operator-=(float scalar){
    for(auto& val : data_){
        val -= scalar;
    }
    return *this;    
}
// 标量复合运算
Matrix& Matrix::operator*=(float scalar){
    for(auto& val : data_){
        val *= scalar;
    }
    return *this;    
}
Matrix& Matrix::operator/=(float scalar){
    for(auto& val : data_){
        val /= scalar;
    }
    return *this;  
}
// 矩阵乘法复合运算（注意：内部形状可能会改变）
Matrix& Matrix::operator*=(const Matrix& rhs){
    if(cols_ != rhs.rows_){
        throw std::invalid_argument("Matrix dimensions do not allow multiplication");
    }
    Matrix result(rows_, rhs.cols_);
    for(int i = 0; i < rows_; ++i){
        for(int j = 0; j < rhs.cols_; ++j){
            float sum = 0.0f;
            for(int k = 0; k < cols_; ++k){
                sum += (*this)(i, k) * rhs(k, j);
            }
            result(i, j) = sum;
        }
    }
    *this = std::move(result);
    return *this;
}
// ==========================================
// 2. 算术运算符（非成员 / 友元函数，按值返回 Matrix）
// ==========================================
// 矩阵与矩阵加减
Matrix operator+(const Matrix& lhs, const Matrix& rhs){
    if(lhs.rows() != rhs.rows() || lhs.cols() != rhs.cols()){
        throw std::invalid_argument("Matrix dimensions must match for addition");
    }
    Matrix result(lhs.rows(), lhs.cols());
    for(int i = 0; i < lhs.rows(); ++i){
        for(int j = 0; j < lhs.cols(); ++j){
            result(i, j) = lhs(i, j) + rhs(i, j);
        }
    }
    return result;
}
Matrix operator-(const Matrix& lhs, const Matrix& rhs){
    if(lhs.rows() != rhs.rows() || lhs.cols() != rhs.cols()){
        throw std::invalid_argument("Matrix dimensions must match for addition");
    }
    Matrix result(lhs.rows(), lhs.cols());
    for(int i = 0; i < lhs.rows(); ++i){
        for(int j = 0; j < lhs.cols(); ++j){
            result(i, j) = lhs(i, j) - rhs(i, j);
        }
    }
    return result;
}
// 矩阵与矩阵乘法（线性代数乘法）
Matrix operator*(const Matrix& lhs, const Matrix& rhs){
    if(lhs.cols() != rhs.rows()){
        throw std::invalid_argument("Matrix dimensions do not allow multiplication");
    }
    Matrix result(lhs.rows(), rhs.cols());
    for(int i = 0; i < lhs.rows(); ++i){
        for(int j = 0; j < rhs.cols(); ++j){
            float sum = 0.0f;
            for(int k = 0; k < lhs.cols(); ++k){
                sum += lhs(i, k) * rhs(k, j);
            }
            result(i, j) = sum;
        }
    }
    return result;
}
// 矩阵与标量乘法（必须支持双向交换律）
Matrix operator*(const Matrix& mat, float scalar){
    Matrix result(mat.rows(), mat.cols());
    for(int i = 0; i < mat.rows(); ++i){
        for(int j = 0; j < mat.cols(); ++j){
            result(i, j) = mat(i, j) * scalar;
        }
    }
    return result;
}
Matrix operator*(float scalar, const Matrix& mat){
    return mat * scalar;
}
// 矩阵与标量除法
Matrix operator/(const Matrix& mat, float scalar){
    Matrix result(mat.rows(), mat.cols());
    for(int i = 0; i < mat.rows(); ++i){
        for(int j = 0; j < mat.cols(); ++j){
            result(i, j) = mat(i, j) / scalar;
        }
    }
    return result;
}

void Matrix::check_bounds(int r, int c) const{
    if(r < 0 || r >= rows_ || c < 0 || c >= cols_){
        throw std::out_of_range("Matrix index out of bounds");
    }
}


// ==========================================
// MatrixManual 类的实现
// ==========================================

MatrixManual::MatrixManual(int rows, int cols)
    : rows_(rows),
      cols_(cols),
      data_(new float[rows * cols]()) {
    if (rows <= 0 || cols <= 0) {
        throw std::invalid_argument("Matrix dimensions must be positive");
    }
}

MatrixManual::~MatrixManual(){
    delete[] data_;   // 核心使命：释放之前 new[] 分配的堆内存
    data_ = nullptr;  // 置空指针，防止悬挂指针
}

MatrixManual::MatrixManual(const MatrixManual& other):
    rows_(other.rows_), cols_(other.cols_), data_(new float[other.rows_ * other.cols_]){
    std::copy(other.data_, other.data_ + (other.rows_ * other.cols_), data_);
}

MatrixManual& MatrixManual::operator=(const MatrixManual& other){
    if(this == &other) return *this; // 自我赋值检查
    // 释放旧内存
    delete[] data_;
    // 分配新内存并复制数据
    rows_ = other.rows_;
    cols_ = other.cols_;
    data_ = new float[rows_ * cols_];
    std::copy(other.data_, other.data_ + (rows_ * cols_), data_);
    return *this;
}

// ==========================================
// MatrixUnique 类的实现
// ==========================================

MatrixUnique::MatrixUnique(int rows, int cols)
    : rows_(rows), cols_(cols), data_(std::make_unique<float[]>(rows * cols)) {
    if (rows <= 0 || cols <= 0) {
        throw std::invalid_argument("Matrix dimensions must be positive");
    }
}

