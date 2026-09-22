#include "Tensor.h"
#include "Matrix.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <iomanip> // 格式化输出

void benchmark_allocation() {
    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < 1000; ++i) {
        TensorBuffer buffer_small({1,256}, "small_tensor"+ std::to_string(i));
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start) / 1000;
    std::cout << "Small Tensor allocation time: " << duration.count() << " microseconds\n";

    start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < 100; ++i) {
        TensorBuffer buffer_mid({1,262144}, "mid_tensor"+ std::to_string(i));
    }
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start) / 100;
    std::cout << "Mid Tensor allocation time: " << duration.count() << " microseconds\n";

    start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < 10; ++i) {
        TensorBuffer buffer_Big({100,262144}, "big_tensor"+ std::to_string(i));
    }
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start) / 10;
    std::cout << "Big Tensor allocation time: " << duration.count() << " microseconds\n";
}   

void benchmark_copy(){
    TensorBuffer src_small({10,262144},"small_source");
    TensorBuffer src_mid({50,262144},"mid_source");
    TensorBuffer src_big({100,262144},"big_source");
    auto start = std::chrono::high_resolution_clock::now();
    TensorBuffer copy_small(src_small);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Time taken to copy small tensor: " << duration.count() << " microseconds\n";

    start = std::chrono::high_resolution_clock::now();
    TensorBuffer copy_mid(src_mid);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Time taken to copy mid tensor: " << duration.count() << " microseconds\n";

    start = std::chrono::high_resolution_clock::now();
    TensorBuffer copy_big(src_big);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Time taken to copy big tensor: " << duration.count() << " microseconds\n";
} 

void benchmark_matmul(int N = 64){
    Matrix A(N,N);
    Matrix B(N,N);
    //warm up
    Matrix C = A * B;
    auto start = std::chrono::high_resolution_clock::now();
    C = A * B;
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    double gflops = (2.0 * N * N * N) / (duration.count() * 1e-6) / 1e9;
    std::cout << "Matrix multiplication of size " << N << "x" << N << " took " << duration.count() << " microseconds, GFLOPS: " << gflops << "\n";
}

int main() {
    benchmark_allocation();
    std::cout << "----------------------------------------" << std::endl;
    benchmark_copy();
    std::cout << "----------------------------------------" << std::endl;
    benchmark_matmul();
    std::cout << "----------------------------------------" << std::endl;
    benchmark_matmul(128);
    std::cout << "----------------------------------------" << std::endl;
    benchmark_matmul(256);
    return 0;
}