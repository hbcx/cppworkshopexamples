#include <cstddef>
#include <iostream>
#include <string>
#include <tuple>
#include <utility>   // std::index_sequence, std::make_index_sequence

// The helper receives the tuple plus a compile-time pack of its indices
// (0, 1, ..., N-1) as std::index_sequence, then expands std::get<Is> over that
// pack to touch every element -- one definition works for any tuple size.
template <typename Tuple, std::size_t... Is>
void printAll(const Tuple& t, std::index_sequence<Is...>) {
    // Pre-fold (C++11/14) pack expansion: a braced-init array evaluates each
    // element left to right, running the print once per index. The leading 0
    // keeps the array non-empty even for a zero-element tuple.
    int expand[] = {0, ((std::cout << "[" << std::get<Is>(t) << "]"), 0)...};
    (void) expand;
    // A C++17 fold expression writes the same thing in one line:
    //     ((std::cout << "[" << std::get<Is>(t) << "]"), ...);
}

template <typename Tuple>
void printTuple(const Tuple& t) {
    // make_index_sequence<N> is index_sequence<0, 1, ..., N-1>, built from the
    // tuple's compile-time size.
    constexpr std::size_t n = std::tuple_size<Tuple>::value;
    printAll(t, std::make_index_sequence<n>{});
}

int main() {
    auto t = std::make_tuple(42, std::string("hi"), 3.14, 'z');
    printTuple(t);
    std::cout << '\n';
    std::cout << "size = " << std::tuple_size<decltype(t)>::value << '\n';
    return 0;
}
