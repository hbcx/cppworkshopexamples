#include <cstdlib>
#include <iostream>
#include <new>
#include <string>

// Count heap allocations by overriding the global operator new, so we can SEE
// which prefix test touches the heap and which does not.
static std::size_t g_allocs = 0;
void* operator new(std::size_t n) {
    ++g_allocs;
    if (void* p = std::malloc(n ? n : 1)) return p;
    throw std::bad_alloc();
}
void operator delete(void* p) noexcept { std::free(p); }
void operator delete(void* p, std::size_t) noexcept { std::free(p); }

int main() {
    std::string s = "https://example.com/api/v1/users";
    std::string prefix = "https://example.com/";      // 20 chars: too long for SSO

    // Anti-pattern: substr copies the first 20 chars into a fresh heap string
    // only to compare and discard it.
    std::size_t before = g_allocs;
    bool viaSubstr = (s.substr(0, prefix.size()) == prefix);
    std::size_t substrAllocs = g_allocs - before;

    // Allocation-free: compare the range in place.
    before = g_allocs;
    bool viaCompare = (s.compare(0, prefix.size(), prefix) == 0);
    std::size_t compareAllocs = g_allocs - before;

    // Allocation-free: anchor the search at position 0.
    before = g_allocs;
    bool viaRfind = (s.rfind(prefix, 0) == 0);
    std::size_t rfindAllocs = g_allocs - before;

    std::cout << "same answer? " << (viaSubstr == viaCompare && viaCompare == viaRfind ? "yes" : "no")
              << " (" << (viaSubstr ? "prefix matches" : "no match") << ")\n";
    std::cout << "substr  test allocations: " << substrAllocs << '\n';   // >= 1
    std::cout << "compare test allocations: " << compareAllocs << '\n';  // 0
    std::cout << "rfind   test allocations: " << rfindAllocs << '\n';    // 0
    return 0;
}
