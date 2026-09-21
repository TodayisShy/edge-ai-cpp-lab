#include "Tensor.h"

#include <cassert>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <chrono>       //高精度计时

void test_construction() {
    TensorBuffer buffer({2, 3},"abc");

    assert(buffer.numel() == 6);
    assert(buffer.shape() == std::vector<int>({2, 3}));
    assert(buffer.at(0) == 0.0f);
    assert(buffer.at(5) == 0.0f);
    std::cout << "TensorBuffer name: " << buffer.name() << std::endl;
}

void test_reshape() {
    TensorBuffer buffer({2, 3},"def");
    buffer.at(4) = 7.5f;

    buffer.reshape({3, 2});

    assert(buffer.shape() == std::vector<int>({3, 2}));
    assert(buffer.at(4) == 7.5f);
    std::cout << "TensorBuffer name: " << buffer.name() << std::endl;
}

void test_copy() {
    TensorBuffer source({2, 2},"ghi");
    source.at(0) = 1.0f;
    source.at(3) = 4.0f;

    TensorBuffer copied(source);
    TensorBuffer assigned({1, 4},"jkl");
    assigned = source;
    source.at(0) = 9.0f;

    assert(copied.at(0) == 1.0f);
    assert(copied.at(3) == 4.0f);
    assert(assigned.at(0) == 1.0f);
    assert(assigned.at(3) == 4.0f);
    std::cout << "Copied TensorBuffer name: " << copied.name() << std::endl;
    std::cout << "Assigned TensorBuffer name: " << assigned.name() << std::endl;
}

void test_bounds_check() {
    TensorBuffer buffer({2, 2},"mno");

    bool threw = false;
    try {
        buffer.at(4);
    } catch (const std::out_of_range&) {
        threw = true;
    }

    assert(threw);
    std::cout << "TensorBuffer name: " << buffer.name() << std::endl;
}

void test_performance_100mb(){
    TensorBuffer big_tensor({262144,100},"big_tensor");
    auto t0 = std::chrono::high_resolution_clock::now();
    TensorBuffer deep_copy_tensor(big_tensor);
    auto t1 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0);
    std::cout << "Time taken to copy 100MB tensor: " << duration.count() << " microseconds" << std::endl;

    std::cout << "big_tensor data_point before move: " << big_tensor.data() << std::endl; 

    auto t2 = std::chrono::high_resolution_clock::now();
    TensorBuffer move_tensor(std::move(big_tensor));
    auto t3 = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(t3 - t2);
    std::cout << "Time taken to move 100MB tensor: " << duration2.count() << " microseconds" << std::endl;

    std::cout << "big_tensor numel after move: " << big_tensor.numel() << std::endl; // Should be 0
    std::cout << "big_tensor data_point after move: " << big_tensor.data() << std::endl; 
    std::cout << "move_tensor data_point after move: " << move_tensor.data() << std::endl; 
}

int main() {
    test_construction();
    std::cout << "----------------------------------------" << std::endl;
    test_reshape();
    std::cout << "----------------------------------------" << std::endl;
    test_copy();
    std::cout << "----------------------------------------" << std::endl;
    test_bounds_check();
    std::cout << "----------------------------------------" << std::endl;
    test_performance_100mb();
    std::cout << "----------------------------------------" << std::endl;
    return 0;
}
