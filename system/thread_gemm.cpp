#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono> 
#include <cmath>
#include "Matrix.h"

#ifdef _WIN32
#include <windows.h>
#endif

void verify_equal(const Matrix& C1, const Matrix& C2, float epsilon = 1e-4f) {
    // 逐元素对比，检查 fabs(C1(i, j) - C2(i, j)) <= epsilon
    if (C1.rows() != C2.rows() || C1.cols() != C2.cols()) {
        throw std::invalid_argument("Matrices have different dimensions");
    }
    for (int i = 0; i < C1.rows(); ++i) {
        for (int j = 0; j < C1.cols(); ++j) {
            if (std::fabs(C1(i, j) - C2(i, j)) > epsilon) {
                throw std::invalid_argument("Matrices are not equal within the specified tolerance");
            }
        }
    }
}

void matmul_worker(const Matrix& A, const Matrix& B, Matrix& C, int start_row, int end_row){
    for(int i = start_row; i < end_row; ++i){
        for(int j = 0; j < B.cols(); ++j){
            float sum = 0.0f;
            for(int k = 0; k < A.cols(); ++k){
                sum += A(i, k) * B(k, j);
            }
            C(i, j) = sum;
        }
    }
}




int main(){
    // 矩阵维度
    int M = 512; // A 的行数
    int K = 1024; // A 的列数 / B 的行数
    int N = 512; // B 的列数

    // 创建矩阵 A, B, C
    Matrix A(M, K);
    Matrix B(K, N);
    Matrix C(M, N); // 用于存储结果
    Matrix C_ref(M, N); // 用于存储参考结果
    // 初始化矩阵 A 和 B
    for(int i = 0; i < M; ++i){
        for(int j = 0; j < K; ++j){
            A(i, j) = static_cast<float>(i + j);
        }
    }
    for(int i = 0; i < K; ++i){
        for(int j = 0; j < N; ++j){
            B(i, j) = static_cast<float>(i - j);
        }
    }

    C_ref = A * B; // 计算参考结果

    // 获取硬件线程数量
    // unsigned int num_threads = std::thread::hardware_concurrency();
    unsigned int num_threads = 1; // 如果无法获取，则默认使用4线程

    std::cout << "Using " << num_threads << " threads for matrix multiplication." << std::endl;

    // 分配每个线程处理的行数
    std::vector<std::thread> threads;
    int rows_per_thread = M / num_threads;
    int remaining_rows = M % num_threads;

    auto start_time = std::chrono::high_resolution_clock::now();

    int current_row = 0;
    for(unsigned int t = 0; t < num_threads; ++t){
        int end_row = current_row + rows_per_thread + (t < remaining_rows ? 1 : 0);
        threads.emplace_back(matmul_worker, std::cref(A), std::cref(B), std::ref(C), current_row, end_row);
        current_row = end_row;
    }

    // 等待所有线程完成
    for(auto& th : threads){
        th.join();
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end_time - start_time;
    std::cout << "Elapsed time: " << elapsed_seconds.count() << " seconds" << std::endl;

    // 验证结果
    try {
        verify_equal(C, C_ref);
        std::cout << "Results are correct!" << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}