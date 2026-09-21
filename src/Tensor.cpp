#include "Tensor.h"

#include <algorithm>
#include <utility>
#include <iostream>

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



// ==========================================
// TensorBuffer 类的实现
// ==========================================

TensorBuffer::TensorBuffer(const std::vector<int>& shape, const std::string& name)
    :name_(name), shape_(shape){
        size_t temp_numel = 1;
        if(shape_.empty()){
            throw std::invalid_argument("TensorBuffer shape cannot be empty");
        }
        for(auto dim : shape_){
            if(dim <= 0){
                throw std::invalid_argument("TensorBuffer dimensions must be positive");
            }  
            temp_numel *= static_cast<size_t>(dim);
        }
        numel_ = temp_numel;
        data_  = std::make_unique<float[]>(numel_);
        std::cout << "[CONSTRUCT]   '" << name_ << "' created (" 
            << numel_ << " floats, " 
            << (numel_ * sizeof(float)) / (1024.0 * 1024.0) << " MB)" << std::endl; 
}

TensorBuffer::~TensorBuffer(){
    std::cout << "[DESTRUCT]  '" << name_ << "' destroyed." << std::endl;
}

TensorBuffer::TensorBuffer(const TensorBuffer& other)
    :name_(other.name_), shape_(other.shape_), numel_(other.numel_), 
    data_(numel_ > 0 ? std::make_unique<float[]>(numel_) : nullptr) {
    if(numel_ && data_){
        std::copy(other.data_.get(), other.data_.get() + numel_, data_.get());
    }
    std::cout << "[COPY CTOR]   '" << other.name_ << "' -> '" << name_ 
              << "' (Deep copied " << numel_ << " floats)" << std::endl;
}

TensorBuffer& TensorBuffer::operator=(const TensorBuffer& other) {
    if (this == &other) {
        return *this;
    }
    // 1. 先开辟新内存，如果抛出异常，当前对象状态完全不受损
    auto new_data = other.numel_ > 0 ? std::make_unique<float[]>(other.numel_) : nullptr;
    if (new_data && other.data_) {
        std::copy(other.data_.get(), other.data_.get() + other.numel_, new_data.get());
    }
    name_ = other.name_;
    shape_ = other.shape_;
    numel_ = other.numel_;
    data_ = std::move(new_data);    
    std::cout << "[COPY ASSIGN] '" << other.name_ << "' -> '" << name_ << "'\n"; 
    return *this;
}

TensorBuffer::TensorBuffer(TensorBuffer&& other) noexcept
    :name_(std::move(other.name_)),
    shape_(std::move(other.shape_)),
    numel_(other.numel_),
    data_(std::move(other.data_)){
    other.numel_ = 0;
    std::cout << "[MOVE CTOR]   '" << other.name_ << "' -> '" << name_ << "' (Moved " << numel_ << " floats)" << std::endl;
}


TensorBuffer& TensorBuffer::operator=(TensorBuffer&& other)noexcept{
    if (this == &other) {
        return *this;
    }
    name_ = std::move(other.name_);
    shape_ = std::move(other.shape_);
    numel_ = other.numel_;
    data_ = std::move(other.data_);
    other.numel_ = 0;
    std::cout << "[MOVE ASSIGN] '" << other.name_ << "' -> '" << name_ << "' (Moved " << numel_ << " floats)" << std::endl;
    return *this;
}

size_t TensorBuffer::numel()const noexcept{
    return numel_;
}

float* TensorBuffer::data() noexcept{
    return data_.get();
}

const float* TensorBuffer::data() const noexcept{
    return data_.get();
}

float& TensorBuffer::at(size_t index){
    check_bounds(index);
    return data_[index];
}

const float& TensorBuffer::at(size_t index) const{
    check_bounds(index);
    return data_[index];
}

void TensorBuffer::reshape(const std::vector<int>& new_shape){
    if(new_shape.empty()){
        throw std::invalid_argument("New shape cannot be empty");
    }

    size_t temp_numel = 1;
    for(auto dim : new_shape){
        if(dim <= 0){
            throw std::invalid_argument("TensorBuffer dimensions must be positive");
        }
        temp_numel *= static_cast<size_t>(dim);
    }
    if(temp_numel != numel_){
        throw std::invalid_argument("New shape must have the same number of elements as the original shape");
    }

    shape_ = new_shape;
}

void TensorBuffer::fill(float value) noexcept{
    std::fill(data_.get(), data_.get() + numel_, value);
}

void TensorBuffer::check_bounds(size_t index) const{
    if(index >= numel_){
        throw std::out_of_range("Index out of bounds");
    }
}