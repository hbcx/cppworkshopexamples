#include <cstdio>
#include <cstdlib>
#include <cstddef>
#include <new>

// Replacing the global allocation functions: the linker picks these over the
// library's, so EVERY new/delete in the whole program now goes through them.
static std::size_t g_allocs = 0;
static std::size_t g_frees  = 0;

void* operator new(std::size_t n) {
    ++g_allocs;
    if (void* p = std::malloc(n ? n : 1)) return p;
    throw std::bad_alloc();                 // operator new must throw, not return null
}
void operator delete(void* p) noexcept {
    if (p) { ++g_frees; std::free(p); }
}
void operator delete(void* p, std::size_t) noexcept {   // sized form (C++14 ABI)
    if (p) { ++g_frees; std::free(p); }
}

int main() {
    // The absolute counters also include allocations made by the standard
    // library, so measure a delta around exactly the allocations we control.
    std::size_t a0 = g_allocs, f0 = g_frees;

    int* p = new int(42);
    int* q = new int(7);
    std::size_t allocsHere = g_allocs - a0;

    delete p;
    delete q;
    std::size_t freesHere = g_frees - f0;

    std::printf("allocations in this block: %zu\n", allocsHere);
    std::printf("frees in this block: %zu\n", freesHere);
}
