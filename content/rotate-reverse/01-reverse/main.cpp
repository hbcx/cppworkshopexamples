#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{1, 2, 3, 4, 5};

    // reverse flips the order of a range IN PLACE, swapping the ends inward.
    std::reverse(v.begin(), v.end());
    std::cout << "reversed in place:   ";
    for (int x : v) std::cout << x << ' ';
    std::cout << '\n';

    // It works on a sub-range too: flip just the first three elements.
    std::reverse(v.begin(), v.begin() + 3);
    std::cout << "first three flipped: ";
    for (int x : v) std::cout << x << ' ';
    std::cout << '\n';

    // reverse_copy writes the reversed result to an OUTPUT and leaves the source
    // untouched.
    std::vector<int> original{10, 20, 30};
    std::vector<int> backwards;
    std::reverse_copy(original.begin(), original.end(),
                      std::back_inserter(backwards));
    std::cout << "reverse_copy:        ";
    for (int x : backwards) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
