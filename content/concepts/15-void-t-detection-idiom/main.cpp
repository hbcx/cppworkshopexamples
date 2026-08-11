#include <concepts>
#include <iostream>
#include <type_traits>
#include <vector>

// BAD: detect "has size()" the pre-C++20 way -- a primary trait, a specialization
// keyed on void_t, and std::declval. Several lines of boilerplate for one query.
template <class, class = void>
struct has_size_old : std::false_type {};
template <class T>
struct has_size_old<T, std::void_t<decltype(std::declval<const T&>().size())>>
    : std::true_type {};

// GOOD: a requires-expression says the same thing in one readable line.
template <class T>
concept HasSize = requires(const T& t) { t.size(); };

int main() {
    std::cout << std::boolalpha;
    std::cout << "vector: void_t=" << has_size_old<std::vector<int>>::value
              << " concept=" << HasSize<std::vector<int>> << "\n";
    std::cout << "int:    void_t=" << has_size_old<int>::value
              << " concept=" << HasSize<int> << "\n";
    std::cout << "done\n";
}
