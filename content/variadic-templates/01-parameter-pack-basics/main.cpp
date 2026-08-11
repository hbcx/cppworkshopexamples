#include <array>
#include <cstddef>
#include <iostream>

// sizeof...(pack) is the element count -- a compile-time constant usable anywhere
// a constant is: a static_assert, an array size, a template argument, a return.
template <class... Ts>
std::size_t count(Ts... /*args*/) {
    static_assert(sizeof...(Ts) < 100, "too many arguments");   // in a static_assert
    std::array<int, sizeof...(Ts)> slots{};                     // as an array size
    return slots.size();                                        // == sizeof...(Ts)
}

int main() {
    std::cout << "count()          = " << count() << "\n";              // 0
    std::cout << "count(1)         = " << count(1) << "\n";             // 1
    std::cout << "count(1,'a',3.0) = " << count(1, 'a', 3.0) << "\n";   // 3, mixed types
    return 0;
}
