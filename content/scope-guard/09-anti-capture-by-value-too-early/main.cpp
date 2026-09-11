#include <cstdlib>
#include <iostream>
#include <utility>

long g_frees = 0;
void countingFree(void* p) {
    if (p) {
        ++g_frees;
        std::free(p);
    }
}

template <typename F>
class Finally {
    F action_;
public:
    explicit Finally(F a) : action_(std::move(a)) {}
    Finally(const Finally&) = delete;
    ~Finally() { action_(); }
};
template <typename F> Finally<F> finally(F a) { return Finally<F>(std::move(a)); }

int main() {
    // ANTI-PATTERN: capture the resource BY VALUE while it is still null. The guard
    // remembers null; the later allocation is never freed by it, and leaks.
    g_frees = 0;
    {
        char* p = nullptr;
        auto bad = finally([p] { countingFree(p); });     // captured p == nullptr
        p = static_cast<char*>(std::malloc(16));           // guard still holds null
        (void) p;
    }
    std::cout << "by value: frees = " << g_frees << " (real buffer leaked)\n";

    // FIX: capture by reference so the guard sees the pointer's final value.
    g_frees = 0;
    {
        char* p = nullptr;
        auto good = finally([&p] { countingFree(p); });
        p = static_cast<char*>(std::malloc(16));
    }
    std::cout << "by ref:   frees = " << g_frees << " (real buffer freed)\n";
    return 0;
}
