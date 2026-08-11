#include <iostream>
#include <vector>

// Deduction works from the ARGUMENTS.
template <class T>
T identity(T x) { return x; }

// R (the return type) appears in no parameter, so it cannot be deduced -- you must
// give it explicitly. T is still deduced from the argument. R is listed first
// because it is the one you supply.
template <class R, class T>
R convert(T x) { return static_cast<R>(x); }

int main() {
    std::cout << "identity(42)  -> " << identity(42) << "\n";     // T deduced as int
    std::cout << "identity(2.5) -> " << identity(2.5) << "\n";    // T deduced as double

    std::cout << "convert<int>(3.9)   -> " << convert<int>(3.9) << "\n";     // R=int    -> 3
    std::cout << "convert<double>(7)  -> " << convert<double>(7) << "\n";    // R=double -> 7

    // A braced list {1,2,3} has no type, so T cannot be deduced from it:
    //   identity({1, 2, 3});   // ERROR: cannot deduce T from a braced-init-list
    std::vector<int> v = {1, 2, 3};
    std::cout << "identity(vector).size() -> " << identity(v).size() << "\n";   // 3
    return 0;
}
