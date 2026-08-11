#include <concepts>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

// BAD: "Addable" only checks that a + b COMPILES and returns T. std::string
// satisfies it, but + means concatenation, not numeric addition -- the concept
// constrains the syntax, not the meaning.
template <class T>
concept Addable = requires(T a, T b) {
    { a + b } -> std::same_as<T>;
};

template <Addable T>
T total(const std::vector<T>& xs) {
    T acc = xs.empty() ? T{} : xs[0];
    for (std::size_t i = 1; i < xs.size(); ++i) acc = acc + xs[i];
    return acc;
}

// GOOD: constrain to actual numbers, so a type whose + means something else
// cannot slip in.
template <class T>
concept Number = std::integral<T> || std::floating_point<T>;

template <Number T>
T sumNumbers(const std::vector<T>& xs) {
    T acc{};
    for (const T& x : xs) acc = acc + x;
    return acc;
}

int main() {
    std::vector<int> nums = {1, 2, 3};
    std::cout << "total(ints)      = " << total(nums) << "\n";      // 6, fine

    std::vector<std::string> words = {"a", "b", "c"};
    std::cout << "total(strings)   = " << total(words) << "\n";     // abc -- concatenation!

    std::cout << "sumNumbers(ints) = " << sumNumbers(nums) << "\n"; // 6
    // sumNumbers(words);  // ERROR: std::string is not a Number -- the semantic
    //                     // concept rejects it, where Addable accepted it.
    std::cout << "done\n";
}
