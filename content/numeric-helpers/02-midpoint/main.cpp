#include <iostream>
#include <numeric>
#include <vector>

int main() {
    // std::midpoint (C++20) computes the average of two numbers, halfway between
    // them. For integers it rounds toward the FIRST argument.
    std::cout << "midpoint(2, 8) = " << std::midpoint(2, 8) << '\n';   // 5
    std::cout << "midpoint(1, 4) = " << std::midpoint(1, 4) << '\n';   // 2 (toward 1)
    std::cout << "midpoint(4, 1) = " << std::midpoint(4, 1) << '\n';   // 3 (toward 4)

    // Its real value is that it never overflows: it does not form a + b, so it is
    // safe even when the two values are near the type's limit (see the anti-pattern
    // on (a + b) / 2). It also works on floating point.
    std::cout << "midpoint(1.0, 2.0) = " << std::midpoint(1.0, 2.0) << '\n';

    // std::midpoint also accepts two POINTERS into the same array and returns the
    // pointer halfway between them -- the safe way to pick a middle element in a
    // binary search without computing (low + high) that could overflow.
    std::vector<int> v{10, 20, 30, 40, 50, 60, 70};
    int* mid = std::midpoint(&v.front(), &v.back());
    std::cout << "middle element = " << *mid << '\n';   // 40
    return 0;
}
