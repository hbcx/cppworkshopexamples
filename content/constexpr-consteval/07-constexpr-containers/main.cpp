#include <array>
#include <cstddef>
#include <iostream>
#include <vector>

// In C++20 std::vector can be used during constant evaluation, so a table is
// computed with ordinary code. Any allocation made at compile time must also be
// freed at compile time, so copy the result into a std::array (a literal type),
// which can survive as a constant.
constexpr std::array<int, 10> firstSquares() {
    std::vector<int> tmp;                                // constexpr allocation (C++20)
    for (int i = 0; i < 10; ++i) tmp.push_back(i * i);

    std::array<int, 10> out{};
    for (std::size_t i = 0; i < tmp.size(); ++i) out[i] = tmp[i];
    return out;                                          // tmp is freed here, still at compile time
}

// Computed once, at compile time; the array is a constant in the binary.
constexpr std::array<int, 10> kSquares = firstSquares();

int main() {
    static_assert(kSquares[9] == 81);
    std::cout << "squares:";
    for (int s : kSquares) std::cout << " " << s;
    std::cout << "\n";
    return 0;
}
