#include <iostream>
#include <vector>
#include <iterator>   // std::begin, std::end

// One template, any range: it only needs begin/end, which the free functions
// provide for both class types and raw arrays.
template <typename Range>
int sum(const Range& r) {
    int total = 0;
    for (auto it = std::begin(r); it != std::end(r); ++it)
        total += *it;
    return total;
}

int main() {
    std::vector<int> v = {1, 2, 3, 4};
    int arr[] = {10, 20, 30};   // a raw C array -- it has no member .begin()

    // std::begin/std::end work on BOTH: for the vector they forward to
    // v.begin()/v.end(); for the array they return the pointers arr and arr + 3,
    // with the length 3 deduced from the array's type.
    std::cout << "sum(vector) = " << sum(v) << "\n";
    std::cout << "sum(array)  = " << sum(arr) << "\n";

    // range-based for follows the same rule: begin()/end() for class types,
    // the array bounds for arrays.
    std::cout << "vector via range-for:";
    for (int x : v) std::cout << " " << x;
    std::cout << "\n";
    return 0;
}
