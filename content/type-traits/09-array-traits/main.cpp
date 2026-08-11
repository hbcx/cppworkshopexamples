#include <iostream>
#include <type_traits>

int main() {
    std::cout << std::boolalpha;

    // rank: how many dimensions does the array type have?
    static_assert(std::rank_v<int> == 0);
    static_assert(std::rank_v<int[5]> == 1);
    static_assert(std::rank_v<int[3][4]> == 2);

    // extent<T, N>: the size of the Nth dimension (0 if unbounded/unknown).
    static_assert(std::extent_v<int[3][4], 0> == 3);
    static_assert(std::extent_v<int[3][4], 1> == 4);
    static_assert(std::extent_v<int[], 0> == 0);         // unbounded -> 0

    // remove_extent peels ONE dimension; remove_all_extents peels them all.
    static_assert(std::is_same_v<std::remove_extent_t<int[3][4]>, int[4]>);
    static_assert(std::is_same_v<std::remove_all_extents_t<int[3][4]>, int>);

    std::cout << "array traits hold\n";
    return 0;
}
