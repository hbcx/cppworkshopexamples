#include <iostream>
#include <type_traits>

struct Plain { int a; double b; };   // trivial and standard-layout

int main() {
    std::cout << std::boolalpha;

    // BAD: std::is_pod bundles two independent properties under one vague name and
    // is DEPRECATED in C++20. (Compiled here as C++17, where it is still fine.)
    std::cout << "is_pod<Plain> = " << std::is_pod<Plain>::value << "\n";   // true

    // GOOD: ask the precise questions the standard split it into.
    static_assert(std::is_trivial_v<Plain>);            // trivial ctor + trivial copy
    static_assert(std::is_standard_layout_v<Plain>);    // predictable memory layout
    static_assert(std::is_trivially_copyable_v<Plain>); // the real "can I memcpy this" question

    std::cout << "trivial=" << std::is_trivial_v<Plain>
              << " standard_layout=" << std::is_standard_layout_v<Plain>
              << " trivially_copyable=" << std::is_trivially_copyable_v<Plain> << "\n";

    // The other classic: std::result_of (deprecated C++17, REMOVED C++20) --
    //   std::result_of<F(Args...)>::type      // old, gone in C++20
    // is replaced by invoke_result:
    //   std::invoke_result_t<F, Args...>      // the modern spelling
    std::cout << "done\n";
    return 0;
}
