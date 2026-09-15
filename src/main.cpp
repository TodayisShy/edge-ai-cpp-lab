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
        Matrix mat(3, 3);

        // 2. 元素赋值（调用非常量版本 operator()）
        float counter = 1.0f;
        for (int i = 0; i < mat.rows(); ++i) {
            for (int j = 0; j < mat.cols(); ++j) {
                mat(i, j) = counter++;
            }
        }

        // 3. 打印矩阵
        print_matrix(mat);

        // 4. 单点读取与修改
        mat(1, 1) = 99.0f;
        std::cout << "\n修改 (1, 1) 后的矩阵:\n";
        print_matrix(mat);

        // 5. 触发越界异常测试防御逻辑
        std::cout << "\n触发越界访问测试: ";
        mat(3, 0) = 42.0f; // 合法下标为 0~2，访问 3 将触发 std::out_of_range

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