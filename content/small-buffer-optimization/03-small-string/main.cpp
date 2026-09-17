#include <cstddef>
#include <cstring>
#include <iostream>

// A miniature string with the small-string optimization: short text lives in an
// inline character buffer, long text goes to the heap. This is what std::string
// does -- short strings cost no allocation.
class SmallString {
    static constexpr std::size_t kInline = 15;   // characters, plus a null byte
    char        inline_[kInline + 1];
    char*       data_;
    std::size_t size_;
    bool        heap_;
public:
    SmallString(const char* s) {
        size_ = std::strlen(s);
        if (size_ <= kInline) {
            data_ = inline_;              // short: stays inside the object
            heap_ = false;
        } else {
            data_ = new char[size_ + 1];  // long: one allocation
            heap_ = true;
        }
        std::memcpy(data_, s, size_ + 1); // copy the text and its null byte
    }
    ~SmallString() { if (heap_) delete[] data_; }
    SmallString(const SmallString&) = delete;
    SmallString& operator=(const SmallString&) = delete;

    const char* c_str()   const { return data_; }
    std::size_t size()    const { return size_; }
    bool        on_heap() const { return heap_; }
};

int main() {
    SmallString shortStr("hello");                        // 5 chars -> inline
    std::cout << shortStr.c_str() << " (len " << shortStr.size()
              << ") on heap? " << shortStr.on_heap() << '\n';          // 0

    SmallString edge("exactly15charss");                  // 15 chars -> inline
    std::cout << edge.c_str() << " (len " << edge.size()
              << ") on heap? " << edge.on_heap() << '\n';              // 0

    SmallString longStr("this text is far too long to fit inline");    // -> heap
    std::cout << longStr.c_str() << " (len " << longStr.size()
              << ") on heap? " << longStr.on_heap() << '\n';           // 1
    return 0;
}
