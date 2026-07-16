#include <array>
#include <iostream>
#include <string>
#include <tuple>

int main() {
    std::array<double, 3> point{1.5, -2.0, 0.25};

    // std::get<I> is checked at COMPILE time -- the index is a template
    // argument, so an out-of-range one is a build error rather than UB:
    //   std::get<3>(point);   // error: static assertion failed
    std::cout << "std::get<0>(point) = " << std::get<0>(point) << '\n';
    std::cout << "std::get<2>(point) = " << std::get<2>(point) << '\n';
    // operator[] cannot make that promise: point[3] compiles and is UB.

    // Structured bindings: the readable way to unpack a fixed-size record. The
    // number of names must match the size, and that is checked too.
    auto [x, y, z] = point;
    std::cout << "unpacked: x=" << x << " y=" << y << " z=" << z << '\n';

    // Bind by reference to WRITE through the names.
    auto& [rx, ry, rz] = point;
    rx = 10.0;
    std::cout << "after rx = 10: point[0] = " << point[0] << '\n';
    (void) ry;
    (void) rz;

    // The size is available to the type system, which is what makes all of the
    // above possible -- a vector cannot do any of it.
    std::cout << "std::tuple_size<decltype(point)>::value = "
              << std::tuple_size<decltype(point)>::value << '\n';

    // It works for any element type, so an array is a decent little record.
    std::array<std::string, 2> name{"Ada", "Lovelace"};
    const auto& [first, last] = name;
    std::cout << "name: " << first << ' ' << last << '\n';
    return 0;
}
