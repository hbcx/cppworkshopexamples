#include <concepts>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

// A tour of the standard concepts: you rarely need to write your own, because
// the library already names the common requirements.
static_assert(std::same_as<int, int>);              // exactly the same type
static_assert(std::convertible_to<short, long>);    // implicit conversion exists
static_assert(std::integral<int> && std::floating_point<double>);
static_assert(std::equality_comparable<std::string>);   // has ==
static_assert(std::totally_ordered<int>);               // has < <= > >=
static_assert(std::copyable<std::vector<int>>);
static_assert(std::regular<int>);                       // default-init + copyable + ==

// std::predicate<P, int>: P is callable with an int and its result is usable as
// bool. Constrain a small algorithm on it.
template <class P>
    requires std::predicate<P, int>
std::size_t countIf(const std::vector<int>& v, P pred) {
    std::size_t n = 0;
    for (int x : v) {
        if (pred(x)) ++n;
    }
    return n;
}

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5, 6};
    std::cout << "even count = " << countIf(v, [](int x) { return x % 2 == 0; }) << "\n";

    // std::invocable<F, Args...>: F can be called with Args... (any return type).
    auto noop = [](int) {};
    static_assert(std::invocable<decltype(noop), int>);

    std::cout << "done\n";
}
