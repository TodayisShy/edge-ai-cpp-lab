#include "Tensor.h"

#include <cassert>
#include <stdexcept>
#include <vector>
#include <iostream>

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

int main() {
    test_construction();
    test_reshape();
    test_copy();
    test_bounds_check();
    return 0;
}
