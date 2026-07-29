#include <cstdlib>
#include <iostream>
#include <new>
#include <string>
#include <string_view>

static std::size_t g_allocs = 0;
void* operator new(std::size_t n) {
    ++g_allocs;
    if (void* p = std::malloc(n ? n : 1)) return p;
    throw std::bad_alloc();
}
void operator delete(void* p) noexcept { std::free(p); }
void operator delete(void* p, std::size_t) noexcept { std::free(p); }

// Three ways to accept a string the function only READS.
static std::size_t byValue(std::string s)              { return s.size(); }  // copies
static std::size_t byConstRef(const std::string& s)    { return s.size(); }  // no copy
static std::size_t byView(std::string_view s)          { return s.size(); }  // no copy

int main() {
    std::string message = "a log line long enough to defeat the small-string buffer";

    std::size_t before = g_allocs;
    byValue(message);
    std::cout << "by value    allocations: " << g_allocs - before << '\n';   // >= 1

    before = g_allocs;
    byConstRef(message);
    std::cout << "by const&   allocations: " << g_allocs - before << '\n';   // 0

    before = g_allocs;
    byView(message);
    std::cout << "by view     allocations: " << g_allocs - before << '\n';   // 0

    // Bonus: a view also takes a literal with no std::string built at all.
    before = g_allocs;
    byView("literal argument, zero allocations");
    std::cout << "view+literal allocations: " << g_allocs - before << '\n';  // 0
    return 0;
}
