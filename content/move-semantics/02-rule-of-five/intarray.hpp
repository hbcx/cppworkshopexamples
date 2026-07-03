#ifndef CPPWS_INTARRAY_HPP
#define CPPWS_INTARRAY_HPP

#include <cstddef>

// Owns a heap-allocated int array. Because it manages a raw resource it must
// define all five special members (the rule of five): destructor, copy
// constructor, copy assignment, move constructor, move assignment.
class IntArray {
public:
    explicit IntArray(std::size_t n) : size_(n), data_(new int[n]()) {}

    // 1. destructor
    ~IntArray() { delete[] data_; }

    // 2. copy constructor: allocate our own storage, copy the elements
    IntArray(const IntArray& other) : size_(other.size_), data_(new int[other.size_]) {
        for (std::size_t i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }

    // 3. copy assignment: build the copy first, then release the old storage
    IntArray& operator=(const IntArray& other) {
        if (this != &other) {
            int* fresh = new int[other.size_];
            for (std::size_t i = 0; i < other.size_; ++i) {
                fresh[i] = other.data_[i];
            }
            delete[] data_;
            data_ = fresh;
            size_ = other.size_;
        }
        return *this;
    }

    // 4. move constructor: steal the pointer, null out the source
    IntArray(IntArray&& other) noexcept : size_(other.size_), data_(other.data_) {
        other.size_ = 0;
        other.data_ = nullptr;
    }

    // 5. move assignment: release our storage, then steal the source's
    IntArray& operator=(IntArray&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = other.data_;
            other.size_ = 0;
            other.data_ = nullptr;
        }
        return *this;
    }

    std::size_t size() const { return size_; }
    void set(std::size_t i, int v) { data_[i] = v; }
    int get(std::size_t i) const { return data_[i]; }

private:
    std::size_t size_;
    int* data_;
};

#endif  // CPPWS_INTARRAY_HPP
