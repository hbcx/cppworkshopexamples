#include <iostream>
#include <utility>

bool g_custom_swap_used = false;

namespace lib {
    struct Buffer {
        int id;
    };
    // Buffer's own swap, found by ADL. Imagine it is the cheap one (swap pointers
    // instead of moving a payload). It records that it ran.
    void swap(Buffer& a, Buffer& b) noexcept {
        std::swap(a.id, b.id);
        g_custom_swap_used = true;
    }
}

// ANTI-PATTERN: hard-qualify std::swap. This always calls the generic library
// swap and never the type's own, so a type with a better swap loses it.
void qualified(lib::Buffer& a, lib::Buffer& b) {
    std::swap(a, b);
}

// FIX: the two-step. using std::swap provides the fallback, then the unqualified
// call lets ADL prefer lib::swap when it exists.
void twoStep(lib::Buffer& a, lib::Buffer& b) {
    using std::swap;
    swap(a, b);
}

int main() {
    lib::Buffer a{1}, b{2};

    g_custom_swap_used = false;
    qualified(a, b);
    std::cout << "std::swap(a,b):            lib::swap used = "
              << std::boolalpha << g_custom_swap_used << "\n";

    g_custom_swap_used = false;
    twoStep(a, b);
    std::cout << "using std::swap; swap(a,b): lib::swap used = "
              << std::boolalpha << g_custom_swap_used << "\n";
    return 0;
}
