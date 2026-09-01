#include <mdspan>
#include <vector>
#include <iostream>
#include <cstddef>

int main() {
    std::vector<int> buf(12);
    for (int i = 0; i < 12; ++i) buf[i] = i;   // 0..11

    // Three ways to describe the same 3x4 shape:

    // (1) All dynamic: both extents are runtime values.
    std::mdspan<int, std::dextents<std::size_t, 2>> a(buf.data(), 3, 4);

    // (2) All static: the shape is part of the type; nothing is stored for it,
    //     and the constructor needs no size arguments.
    std::mdspan<int, std::extents<std::size_t, 3, 4>> b(buf.data());

    // (3) Mixed: rows fixed at 3, columns decided at runtime (dynamic_extent).
    std::mdspan<int, std::extents<std::size_t, 3, std::dynamic_extent>> c(buf.data(), 4);

    std::cout << "a[2,3] = " << a[2, 3] << "\n";
    std::cout << "b[2,3] = " << b[2, 3] << "\n";
    std::cout << "c[2,3] = " << c[2, 3] << "\n";

    // The static shape stores no extents; the dynamic one stores two size_t.
    std::cout << "sizeof all-static  view = " << sizeof(b) << "\n";
    std::cout << "sizeof all-dynamic view = " << sizeof(a) << "\n";
}
