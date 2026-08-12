#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    // A lambda is an object of an anonymous class type with an operator(). Store
    // it in auto -- each lambda has its own unique, unnamable type -- and call it
    // like a function.
    auto square = [](int x) { return x * x; };
    std::cout << "square(5) = " << square(5) << '\n';

    // The everyday use: hand a small piece of behavior straight to an algorithm.
    std::vector<int> v{5, 2, 8, 1, 9, 3};
    std::sort(v.begin(), v.end(), [](int a, int b) { return a > b; });  // descending
    std::cout << "sorted desc: ";
    for (int x : v) std::cout << x << ' ';
    std::cout << '\n';

    int count = std::count_if(v.begin(), v.end(), [](int x) { return x > 3; });
    std::cout << "count > 3: " << count << '\n';
    return 0;
}
