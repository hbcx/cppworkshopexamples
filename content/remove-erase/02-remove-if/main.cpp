#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{10, 3, 25, 4, 7, 40, 2};

    // remove_if is remove with a predicate: it keeps the elements for which the
    // predicate is FALSE, moves them to the front, and returns the new end. Pair
    // it with erase to drop the ones that matched -- here, the even numbers.
    v.erase(std::remove_if(v.begin(), v.end(),
                           [](int n) { return n % 2 == 0; }),
            v.end());

    std::cout << "odd numbers left: ";
    for (int x : v) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
