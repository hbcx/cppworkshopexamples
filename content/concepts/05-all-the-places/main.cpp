#include <concepts>
#include <iostream>

// The SAME constraint (std::integral) written in every place it can appear.
// All six forms accept exactly the same set of types.

// 1) constrained template parameter
template <std::integral T>
T a(T x) { return x; }

// 2) requires-clause after the template header
template <class T> requires std::integral<T>
T b(T x) { return x; }

// 3) trailing requires-clause, after the parameter list
template <class T>
T c(T x) requires std::integral<T> { return x; }

// 4) abbreviated function template: the concept before `auto`
std::integral auto d(std::integral auto x) { return x; }

// 5) constrained RETURN type
std::integral auto make() { return 7; }

int main() {
    std::cout << a(1) << b(2) << c(3) << d(4) << "\n";   // 1234

    // 6) constrained VARIABLE (placeholder type with a concept)
    std::integral auto x = make();
    std::cout << "x = " << x << "\n";

    // Every form rejects a non-integral argument at the call site: a(3.14),
    // b(3.14) and d(3.14) would all fail to compile with the same reason.
    std::cout << "done\n";
}
