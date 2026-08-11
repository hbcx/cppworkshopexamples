#include <iostream>
#include <type_traits>

int main() {
    // conditional_t<Cond, A, B> is A when Cond is true, else B -- an if for types.
    static_assert(std::is_same_v<std::conditional_t<true,  int, double>, int>);
    static_assert(std::is_same_v<std::conditional_t<false, int, double>, double>);

    // A real choice: a wider counter on 64-bit builds, a narrower one otherwise.
    using Counter = std::conditional_t<(sizeof(void*) >= 8), long long, int>;
    static_assert(std::is_integral_v<Counter>);

    // common_type_t is the type a ? b : c settles on -- the shared type several
    // types promote to.
    static_assert(std::is_same_v<std::common_type_t<int, double>, double>);
    static_assert(std::is_same_v<std::common_type_t<char, short, int>, int>);

    std::cout << "conditional and common_type hold\n";
    return 0;
}
