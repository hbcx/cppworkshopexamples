#include <cstddef>
#include <iostream>
#include <new>

// The core idea of the small-buffer optimization: keep a fixed byte buffer
// INSIDE the object, hand out that inline buffer for small requests, and fall
// back to the heap only when the request does not fit. Small data then lives
// where the object lives -- on the stack here -- with no allocation.
class SmallStorage {
    static constexpr std::size_t kInline = 32;
    alignas(std::max_align_t) unsigned char buffer_[kInline];
    unsigned char* data_ = nullptr;
    bool           heap_ = false;
public:
    void* allocate(std::size_t n) {
        if (n <= kInline) {
            data_ = buffer_;                  // point at our own inline bytes
            heap_ = false;
        } else {
            data_ = new unsigned char[n];     // too big: go to the heap
            heap_ = true;
        }
        return data_;
    }
    bool        on_heap() const       { return heap_; }
    const void* data() const          { return data_; }
    const void* inline_buffer() const { return buffer_; }
    ~SmallStorage() { if (heap_) delete[] data_; }
};

int main() {
    SmallStorage small;
    small.allocate(16);
    std::cout << "16 bytes on heap? " << small.on_heap() << '\n';                 // 0
    std::cout << "  data IS the inline buffer? "
              << (small.data() == small.inline_buffer()) << '\n';                 // 1

    SmallStorage big;
    big.allocate(1000);
    std::cout << "1000 bytes on heap? " << big.on_heap() << '\n';                 // 1
    std::cout << "  data IS the inline buffer? "
              << (big.data() == big.inline_buffer()) << '\n';                     // 0

    // The small object's bytes live inside the object itself, so no operator new
    // ran for it. The price is that every SmallStorage always carries the buffer.
    std::cout << "sizeof(SmallStorage) = " << sizeof(SmallStorage) << " bytes\n";
    return 0;
}
