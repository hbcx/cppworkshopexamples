#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{1, 2, 3};

    // next_permutation rearranges the range into the NEXT permutation in
    // lexicographic order and returns true. From 1 2 3 the next is 1 3 2.
    std::cout << "start: 1 2 3\n";
    std::next_permutation(v.begin(), v.end());
    std::cout << "next:  ";
    for (int x : v) std::cout << x << ' ';
    std::cout << '\n';

    // From the LAST permutation (3 2 1) it wraps around to the first (1 2 3) and
    // returns FALSE -- that false is how a loop knows it has seen them all.
    std::vector<int> last{3, 2, 1};
    bool more = std::next_permutation(last.begin(), last.end());
    std::cout << "after 3 2 1 -> ";
    for (int x : last) std::cout << x << ' ';
    std::cout << "(returned " << std::boolalpha << more << ")\n";
    return 0;
}
