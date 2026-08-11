#include <concepts>
#include <iostream>
#include <type_traits>

// BAD: constrain with std::enable_if SFINAE. The extra template parameter with a
// defaulted value is the classic trick -- verbose, easy to misplace, and it gives
// worse errors than a named concept.
template <class T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
T twiceOld(T x) { return x + x; }

// GOOD: the same constraint as one word before auto.
auto twiceNew(std::integral auto x) { return x + x; }

int main() {
    std::cout << "enable_if: " << twiceOld(21) << "\n";   // 42
    std::cout << "concept:   " << twiceNew(21) << "\n";   // 42

    // Both reject a double, but only the concept says so clearly at the call:
    //   twiceOld(3.14);  // SFINAE removes the overload -> "no matching function",
    //                    // with the enable_if machinery spilled into the message.
    //   twiceNew(3.14);  // "constraints not satisfied: std::integral<double>".
    std::cout << "done\n";
}
