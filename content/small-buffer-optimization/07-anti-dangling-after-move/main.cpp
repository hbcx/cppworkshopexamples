#include <cstddef>
#include <iostream>
#include <new>
#include <utility>

// A trimmed inline-only small vector (stays within N) with a correct move that
// move-constructs the inline elements into the destination.
template <class T, std::size_t N>
class SmallVec {
    alignas(T) unsigned char inline_[N * sizeof(T)];
    T*          data_;
    std::size_t size_ = 0;
    T* inline_ptr() { return reinterpret_cast<T*>(inline_); }
public:
    SmallVec() : data_(inline_ptr()) {}
    void push(const T& x) { ::new (data_ + size_) T(x); ++size_; }
    SmallVec(SmallVec&& o) noexcept : data_(inline_ptr()), size_(o.size_) {
        for (std::size_t i = 0; i < size_; ++i) {
            ::new (data_ + i) T(std::move(o.data_[i]));
            o.data_[i].~T();
        }
        o.size_ = 0;
    }
    ~SmallVec() { for (std::size_t i = 0; i < size_; ++i) data_[i].~T(); }
    T*       addr(std::size_t i)       { return data_ + i; }
    const T& operator[](std::size_t i) const { return data_[i]; }
};

int main() {
    SmallVec<int, 4> a;
    a.push(10);
    a.push(20);

    // Grab a pointer to an element WHILE the data lives in a's inline buffer.
    int* p = a.addr(1);
    std::cout << "*p before move = " << *p << '\n';   // 20

    // --- the trap ---
    // Move a into b. With a heap vector the buffer would move with the data and
    // p would still be valid. With SBO the inline bytes stay in `a`; the data is
    // move-constructed into b's OWN inline buffer, so p now points into the
    // moved-from source, not at the live element.
    SmallVec<int, 4> b = std::move(a);

    // Fully defined check (no dereference of the stale pointer): the live element
    // is at a DIFFERENT address than p held.
    std::cout << "p still points at the live element? " << (p == &b[1]) << '\n';   // 0

    // --- the fix: re-fetch the pointer after any move (or do not hold pointers
    // into SBO storage across operations that can move or grow it) ---
    int* fixed = b.addr(1);
    std::cout << "re-fetched pointer is valid: " << (*fixed == 20) << '\n';         // 1
    return 0;
}
