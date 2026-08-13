#include <algorithm>
#include <vector>
#include <iterator>
#include <iostream>

int main() {
    std::vector<int> nums{4, 7, 2, 9, 1, 8, 5, 6};

    // copy_if copies only the elements that satisfy a predicate to the output -- a
    // filter into a new container.
    std::vector<int> evens;
    std::copy_if(nums.begin(), nums.end(), std::back_inserter(evens),
        [](int x){ return x % 2 == 0; });
    std::cout << "evens: ";
    for (int x : evens) std::cout << x << ' ';
    std::cout << '\n';   // 4 2 8 6

    // It writes to an OUTPUT and leaves the source untouched -- nums is unchanged.
    std::cout << "source still has " << nums.size() << " elements\n";   // 8
    return 0;
}
