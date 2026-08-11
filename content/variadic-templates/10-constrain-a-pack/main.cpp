#include <concepts>
#include <iostream>
#include <type_traits>

// Constrain a whole pack: every element must satisfy a requirement. A fold over
// && checks the condition for each type.
template <class... Ts>
concept AllIntegral = (std::integral<Ts> && ...);   // fold over a concept

template <class... Ts>
    requires AllIntegral<Ts...>
auto sumIntegers(Ts... args) {
    return (args + ...);
}

// The trait form of the same idea, as a static_assert inside the body.
template <class... Ts>
auto sumChecked(Ts... args) {
    static_assert((std::is_integral_v<Ts> && ...), "all arguments must be integers");
    return (args + ...);
}

int main() {
    std::cout << "sumIntegers(1,2,3) = " << sumIntegers(1, 2, 3) << "\n";   // 6
    std::cout << "sumChecked(4,5,6)  = " << sumChecked(4, 5, 6) << "\n";    // 15
    // sumIntegers(1, 2.5);   // rejected: 2.5 is not integral -> constraint fails

    static_assert(AllIntegral<int, long, char>);
    static_assert(!AllIntegral<int, double>);
    return 0;
}
