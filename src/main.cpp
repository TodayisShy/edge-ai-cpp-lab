#include <iostream>
#include <iomanip>
#include "Matrix.h"

#ifdef _WIN32
#include <windows.h>
#endif

// 接收常引用的打印函数，用于检验 const 访问接口与 const 正确性
void print_matrix(const Matrix& m) {
    std::cout << "Matrix (" << m.rows() << " x " << m.cols() << "):\n";
    for (int i = 0; i < m.rows(); ++i) {
        std::cout << "  [ ";
        for (int j = 0; j < m.cols(); ++j) {
            std::cout << std::setw(6) << std::fixed << std::setprecision(1) << m(i, j) << " ";
        }
        std::cout << "]\n";
    }
}

int main() {

    #ifdef _WIN32
    // 设置控制台输出编码为 UTF-8
    SetConsoleOutputCP(CP_UTF8);
    #endif

    try {
        // 1. 构造一个 3 行 3 列的矩阵
        Matrix mat1(3, 3);
        Matrix mat2(3, 5);
        // 2. 元素赋值（调用非常量版本 operator()）
        float counter = 1.0f;
        for (int i = 0; i < mat1.rows(); ++i) {
            for (int j = 0; j < mat1.cols(); ++j) {
                mat1(i, j) = counter++;
            }
        }

        for (int i = 0; i < mat2.rows(); ++i) {
            for (int j = 0; j < mat2.cols(); ++j) {
                mat2(i, j) = counter--;
            }
        }

        // 3. 打印矩阵
        print_matrix(mat1);
        print_matrix(mat2);

        Matrix mat3 = mat1 * mat2;
        std::cout << "\n矩阵相乘结果:\n";
        print_matrix(mat3);
        // 4. 单点读取与修改
        mat1(1, 1) = 99.0f;
        std::cout << "\n修改 (1, 1) 后的矩阵:\n";
        print_matrix(mat1);

        // 5. 触发越界异常测试防御逻辑
        std::cout << "\n触发越界访问测试: ";
        mat1(3, 0) = 42.0f; // 合法下标为 0~2，访问 3 将触发 std::out_of_range

    } catch (const std::exception& e) {
        std::cerr << "\n[捕获异常] " << e.what() << '\n';
    }

    // 6. 触发构造非法参数异常
    try {
        std::cout << "\n尝试构造负数维度的矩阵: ";
        Matrix invalid_mat(-2, 4);
    } catch (const std::exception& e) {
        std::cerr << "\n[捕获异常] " << e.what() << '\n';
    }

    return 0;
}