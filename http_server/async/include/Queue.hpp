#pragma once

#include <memory>

template <typename T>
class Queue {
 private:
    std::unique_pointer<T>* data;
    size_t start;
    size_t size;
    size_t actualSize;

 public:
    explicit Queue(size_t size = 8);
    ~Queue();

    std::unique_pointer<T> PopFront();
    void PushBack(std::unique_ptr<T> value);
};
