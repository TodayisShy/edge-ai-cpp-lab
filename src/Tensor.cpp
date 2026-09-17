#include "Tensor.h"

Tensor::Tensor(std::vector<int> shape) : shape_(std::move(shape)){
    if(shape_.empty()){
        throw std::invalid_argument("Tensor shape cannot be empty");
    }
    for(auto dim : shape_){
        if(dim <= 0){
            throw std::invalid_argument("Tensor dimensions must be positive");
        }
    }
    data_.assign(numel(), 0.0f);
}

size_t Tensor::numel() const noexcept {
    size_t n = 1;
    for (auto dim : shape_) {
        n *= static_cast<size_t>(dim);
    }
    return n;
}

const std::vector<int>& Tensor::shape() const noexcept{
    return shape_;
}

float* Tensor::data() noexcept{
    return data_.data();
}

const float* Tensor::data() const noexcept{
    return data_.data();
}

void Tensor::reshape(const std::vector<int>& new_shape){
    if(new_shape.empty()){
        throw std::invalid_argument("New shape cannot be empty");
    }

    size_t temp_numel = 1;
    for(auto dim : new_shape){
        if(dim <= 0){
            throw std::invalid_argument("Tensor dimensions must be positive");
        }
        temp_numel *= static_cast<size_t>(dim);
    }
    if(temp_numel != numel()){
        throw std::invalid_argument("New shape must have the same number of elements as the original shape");
    }

    shape_ = new_shape;
}
