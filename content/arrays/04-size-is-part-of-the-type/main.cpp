#include <array>
#include <cstddef>
#include <iostream>
#include <numeric>
#include <vector>

// Option 1: fixed size. Exact, and exactly as inflexible as it looks.
static int sum_fixed(const std::array<int, 5>& a) {
    return std::accumulate(a.begin(), a.end(), 0);
}

// Option 2: template on N. Generic and zero cost, but the compiler stamps out
// one copy of this function per size you call it with.
template <std::size_t N>
static int sum_any_size(const std::array<int, N>& a) {
    return std::accumulate(a.begin(), a.end(), 0);
}

// Option 3: iterators. The function stops caring what the container is -- this
// is how the standard algorithms are written, and it works for vector too.
template <typename It>
static int sum_range(It first, It last) {
    return std::accumulate(first, last, 0);
}

// Option 4: pointer + length. One instantiation, can live in a .cpp file, and
// the size is back to being a promise nobody checks.
static int sum_raw(const int* data, std::size_t count) {
    int total = 0;
    for (std::size_t i = 0; i < count; ++i) {
        total += data[i];
    }
    return total;
}

int main() {
    std::array<int, 5> five{1, 2, 3, 4, 5};
    std::array<int, 3> three{10, 20, 30};

    std::cout << "sum_fixed(five)  = " << sum_fixed(five) << '\n';
    // sum_fixed(three);   // does not compile: array<int,3> is an unrelated type
    //                     // to array<int,5>, and there is no conversion

    std::cout << "sum_any_size(five)  = " << sum_any_size(five) << '\n';
    std::cout << "sum_any_size(three) = " << sum_any_size(three)
              << "   (a second instantiation was generated for N=3)\n";

    std::cout << "sum_range(five)  = " << sum_range(five.begin(), five.end()) << '\n';
    std::cout << "sum_range(three) = " << sum_range(three.begin(), three.end()) << '\n';

    // The iterator version costs nothing to reuse on a different container:
    std::vector<int> v{100, 200};
    std::cout << "sum_range(vector) = " << sum_range(v.begin(), v.end()) << '\n';

    std::cout << "sum_raw(five) = " << sum_raw(five.data(), five.size()) << '\n';

    // In C++20 this is what std::span is for: one non-owning view that takes an
    // array of any N, a vector, or a C array, and keeps the length with it:
    //   int sum_span(std::span<const int> s);
    return 0;
}
