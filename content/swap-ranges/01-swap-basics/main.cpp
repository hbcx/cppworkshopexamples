#include <utility>   // std::swap
#include <vector>
#include <iostream>

int main() {
    int a = 1, b = 2;

    // std::swap exchanges two values. For movable types it moves rather than
    // copies, so it stays cheap even for expensive objects.
    std::swap(a, b);
    std::cout << "a=" << a << " b=" << b << '\n';   // a=2 b=1

    // Containers provide a member swap that is O(1): it exchanges internal pointers
    // (the buffer, size, capacity), touching none of the elements.
    std::vector<int> x{1, 2, 3};
    std::vector<int> y{9, 8};
    x.swap(y);   // constant time, no element moves
    std::cout << "x.size()=" << x.size() << " y.size()=" << y.size() << '\n';  // 2 3

    // For generic code that swaps values of an unknown type, the idiom is to bring
    // std::swap into scope with a using-declaration and then call swap UNQUALIFIED,
    // so a type's own overload (found by ADL) is preferred when it has one.
    using std::swap;
    swap(a, b);
    std::cout << "after unqualified swap: a=" << a << " b=" << b << '\n';  // a=1 b=2
    return 0;
}
