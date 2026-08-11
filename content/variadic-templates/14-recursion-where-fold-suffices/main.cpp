#include <iostream>

// BAD: the pre-C++17 recursive way to sum a pack -- a base case, a recursive
// overload, and a new template instantiation per pack length.
int sumRec() { return 0; }
template <class First, class... Rest>
auto sumRec(First first, Rest... rest) {
    return first + sumRec(rest...);
}

// GOOD: a fold expression does the same in one line -- no base case, no recursion,
// and correct for the empty pack.
template <class... Ts>
auto sumFold(Ts... args) {
    return (0 + ... + args);
}

int main() {
    std::cout << "sumRec(1,2,3,4)  = " << sumRec(1, 2, 3, 4) << "\n";    // 10
    std::cout << "sumFold(1,2,3,4) = " << sumFold(1, 2, 3, 4) << "\n";   // 10
    std::cout << "sumFold()        = " << sumFold() << "\n";            // 0

    // Both give the same answer; the fold is shorter, needs no base case, and
    // generates far fewer template instantiations. In C++17+, prefer the fold.
    return 0;
}
