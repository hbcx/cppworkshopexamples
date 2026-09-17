#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <new>
#include <vector>

// Count every global allocation, to measure exactly how many heap allocations
// the small-buffer optimization removes on the common (small) path.
static int g_allocs = 0;
void* operator new(std::size_t n) {
    ++g_allocs;
    if (void* p = std::malloc(n ? n : 1)) return p;
    throw std::bad_alloc();
}
void* operator new[](std::size_t n) {
    ++g_allocs;
    if (void* p = std::malloc(n ? n : 1)) return p;
    throw std::bad_alloc();
}
void operator delete(void* p) noexcept              { std::free(p); }
void operator delete(void* p, std::size_t) noexcept { std::free(p); }
void operator delete[](void* p) noexcept              { std::free(p); }
void operator delete[](void* p, std::size_t) noexcept { std::free(p); }

// A tiny inline-or-heap byte holder (the mechanic from example 01, trimmed).
template <std::size_t N>
class SmallBuf {
    alignas(std::max_align_t) unsigned char inline_[N];
    unsigned char* data_;
    bool           heap_;
public:
    explicit SmallBuf(std::size_t n) {
        if (n <= N) { data_ = inline_;              heap_ = false; }
        else        { data_ = new unsigned char[n]; heap_ = true;  }
    }
    ~SmallBuf() { if (heap_) delete[] data_; }
};

int main() {
    g_allocs = 0;
    { SmallBuf<64> s(40); }
    int small = g_allocs;

    g_allocs = 0;
    { SmallBuf<64> s(500); }
    int large = g_allocs;

    g_allocs = 0;
    { std::vector<unsigned char> v(40); }
    int vec = g_allocs;

    std::cout << "SmallBuf, small payload -> allocations: " << small << '\n';    // 0
    std::cout << "SmallBuf, large payload -> allocations: " << large << '\n';    // 1
    std::cout << "std::vector, small payload -> allocations: " << vec << '\n';   // 1
    std::cout << "cost: sizeof(SmallBuf<64>) = " << sizeof(SmallBuf<64>)
              << " bytes (the inline buffer is always carried)\n";
    return 0;
}
