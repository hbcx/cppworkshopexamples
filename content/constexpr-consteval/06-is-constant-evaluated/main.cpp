#include <iostream>
#include <type_traits>

// std::is_constant_evaluated() lets ONE function take two paths: a compile-time
// path (simple, obviously correct) and a run-time path (fast). It must be used
// with a plain `if`, never `if constexpr` (see the anti-pattern for why).
constexpr int powerOfTwo(int n) {
    if (std::is_constant_evaluated()) {
        int r = 1;                          // compile-time path: a clear loop
        for (int i = 0; i < n; ++i) r *= 2;
        return r;
    } else {
        return 1 << n;                      // run-time path: a fast bit shift
    }
}

int main() {
    constexpr int a = powerOfTwo(10);       // compile time -> the loop path
    static_assert(a == 1024);
    std::cout << "compile-time: " << a << "\n";

    int n = 10;
    std::cout << "run-time:     " << powerOfTwo(n) << "\n";   // -> the shift path
    return 0;
}
