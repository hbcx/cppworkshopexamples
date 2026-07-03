#include <algorithm>
#include <cstddef>
#include <iostream>
#include <utility>

// A tiny owning buffer that logs whether it was copied or moved, so you can
// actually see which special member the compiler picks.
class Buffer {
public:
    explicit Buffer(std::size_t size)
        : size_(size), data_(new int[size]()) {
        std::cout << "ctor(" << size_ << ")\n";
    }

    // Copy: allocate our own storage and duplicate every element.
    Buffer(const Buffer& other)
        : size_(other.size_), data_(new int[other.size_]) {
        std::copy(other.data_, other.data_ + size_, data_);
        std::cout << "copy ctor\n";
    }

    // Move: steal the pointer, leave the source empty but valid to destroy.
    Buffer(Buffer&& other) noexcept
        : size_(other.size_), data_(other.data_) {
        other.size_ = 0;
        other.data_ = nullptr;
        std::cout << "move ctor\n";
    }

    ~Buffer() { delete[] data_; }

    std::size_t size() const { return size_; }

private:
    std::size_t size_;
    int* data_;
};

int main() {
    Buffer a(4);              // ctor(4)
    Buffer b = a;             // copy ctor  -- a is a named lvalue
    Buffer c = std::move(a);  // move ctor  -- std::move casts a to an rvalue

    std::cout << "b.size=" << b.size() << " c.size=" << c.size() << '\n';
    return 0;
}
