#include <cstddef>
#include <iostream>
#include <new>
#include <string>
#include <utility>

// A vector-like container with inline capacity N: the first N elements live in
// an inline buffer, and only growth past N touches the heap.
template <class T, std::size_t N>
class SmallVector {
    alignas(T) unsigned char inline_[N * sizeof(T)];
    T*          data_;
    std::size_t size_ = 0;
    std::size_t cap_  = N;

    T*   inline_ptr()       { return reinterpret_cast<T*>(inline_); }
    bool is_inline()  const { return data_ == reinterpret_cast<const T*>(inline_); }

    void grow(std::size_t newCap) {
        T* fresh = static_cast<T*>(::operator new(newCap * sizeof(T)));
        for (std::size_t i = 0; i < size_; ++i) {
            ::new (fresh + i) T(std::move(data_[i]));   // move each element over
            data_[i].~T();
        }
        if (!is_inline()) ::operator delete(data_);
        data_ = fresh;
        cap_  = newCap;
    }
public:
    SmallVector() : data_(inline_ptr()) {}
    SmallVector(const SmallVector&) = delete;            // move is covered in 05
    SmallVector& operator=(const SmallVector&) = delete;
    ~SmallVector() {
        for (std::size_t i = 0; i < size_; ++i) data_[i].~T();
        if (!is_inline()) ::operator delete(data_);
    }

    void push_back(const T& v) {
        if (size_ == cap_) grow(cap_ * 2);
        ::new (data_ + size_) T(v);
        ++size_;
    }
    std::size_t size()     const { return size_; }
    std::size_t capacity() const { return cap_; }
    bool        on_heap()  const { return !is_inline(); }
    const T&    operator[](std::size_t i) const { return data_[i]; }
};

int main() {
    SmallVector<std::string, 3> v;
    v.push_back("alpha");
    v.push_back("beta");
    v.push_back("gamma");
    std::cout << "size " << v.size() << ", on heap? " << v.on_heap() << '\n';   // 3, 0

    v.push_back("delta");   // exceeds inline capacity 3 -> spills to the heap
    std::cout << "size " << v.size() << ", on heap? " << v.on_heap()
              << ", capacity " << v.capacity() << '\n';                          // 4, 1, 6

    std::cout << "first=" << v[0] << " last=" << v[3] << '\n';
    return 0;
}
