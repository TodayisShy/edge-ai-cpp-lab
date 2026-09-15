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

void Matrix::check_bounds(int r, int c) const{
    if(r < 0 || r >= rows_ || c < 0 || c >= cols_){
        throw std::out_of_range("Matrix index out of bounds");
    }
}
