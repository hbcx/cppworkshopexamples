#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v(6);   // six value-initialized ints (all 0)

    // fill sets every element of a range to one value.
    std::fill(v.begin(), v.end(), 7);
    std::cout << "all sevens:         ";
    for (int x : v) std::cout << x << ' ';
    std::cout << '\n';

    // It applies to a sub-range too: set only the first three back to 0.
    std::fill(v.begin(), v.begin() + 3, 0);
    std::cout << "first three zeroed: ";
    for (int x : v) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
