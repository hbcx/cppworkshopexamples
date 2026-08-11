#include <iostream>
#include <type_traits>

// A class that lives entirely at compile time: constexpr members, static
// constexpr data, nested using aliases, and dimensions carried as
// std::integral_constant types.
template <int Rows, int Cols>
struct Grid {
    using value_type = double;                          // nested type alias
    using rows = std::integral_constant<int, Rows>;     // a number carried as a TYPE
    using cols = std::integral_constant<int, Cols>;

    static constexpr int  size   = Rows * Cols;         // static constexpr data
    static constexpr bool square = (Rows == Cols);

    constexpr int index(int r, int c) const { return r * Cols + c; }   // constexpr member
};

int main() {
    constexpr Grid<3, 4> g;

    // Everything is resolved during compilation.
    static_assert(g.size == 12, "3x4 has 12 cells");
    static_assert(Grid<3, 4>::rows::value == 3, "rows carries 3");
    static_assert(Grid<3, 4>::cols::value == 4, "cols carries 4");
    static_assert(std::is_same<Grid<3, 4>::value_type, double>::value, "value_type is double");
    static_assert(!Grid<3, 4>::square, "3x4 is not square");
    static_assert(Grid<5, 5>::square, "5x5 is square");
    static_assert(g.index(1, 2) == 6, "row 1, col 2 in a 4-wide grid is index 6");

    // The same members also work normally at run time.
    std::cout << std::boolalpha
              << "Grid<3,4>: size=" << g.size
              << " square=" << g.square
              << " index(1,2)=" << g.index(1, 2) << "\n";
    std::cout << "rows as a value = " << Grid<3, 4>::rows::value << "\n";
    return 0;
}
