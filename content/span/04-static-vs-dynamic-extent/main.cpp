#include <array>
#include <iostream>
#include <span>
#include <vector>

// Static extent: the size is part of the TYPE. This accepts exactly three ints
// -- a std::array<int,3> or int[3] -- and a wrong size is a compile error, not a
// runtime check.
int cross_x(std::span<const int, 3> a, std::span<const int, 3> b) {
    return a[1] * b[2] - a[2] * b[1];
}

int main() {
    std::array<int, 3> u{1, 2, 3};
    std::array<int, 3> w{4, 5, 6};
    std::cout << "cross x-component: " << cross_x(u, w) << '\n';

    // Dynamic extent (the default): the size is a runtime value in the span.
    std::vector<int> v{1, 2, 3, 4, 5};
    std::span<int> dyn(v);
    std::cout << "dynamic size: " << dyn.size() << '\n';

    // A static-extent span carries its size in the type, so it stores only a
    // pointer, where a dynamic span stores a pointer plus a size.
    std::span<int, 5> stat(v);
    std::cout << "static extent value: " << stat.extent << '\n';
    std::cout << "sizeof dynamic span: " << sizeof(dyn) << '\n';
    std::cout << "sizeof static span:  " << sizeof(stat) << '\n';
    return 0;
}
