#include "Tensor.h"
#include <iostream>

void buggy_function() {
    TensorBuffer a({2, 2}, "Source");
    TensorBuffer b = std::move(a); // a 的资源已被转移，a.data() 变成了 nullptr

    std::cout << "准备向 a 写入数据...\n";
    
    // 💥 致命罪魁祸首：解引用空指针！
    float* ptr = a.data(); 
    *ptr = 123.45f; // 相当于 *nullptr = 123.45f，必定触发段错误！
    
    std::cout << "写入成功（这行绝对执行不到）\n";
}

int main() {
    buggy_function();
    return 0;
}