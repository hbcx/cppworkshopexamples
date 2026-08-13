#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> a{1, 2, 3, 5, 8, 13};   // sorted
    std::vector<int> b{2, 5, 8, 21};          // sorted

    // BAD: a nested loop (a find per element) to compute the intersection. For each
    // element of a it scans b -- O(n*m) -- and ignores that both are already
    // sorted, so it does far more work than needed.
    std::vector<int> slow;
    for (int x : a)
        if (std::find(b.begin(), b.end(), x) != b.end())
            slow.push_back(x);

    // GOOD: set_intersection walks both sorted ranges once in parallel -- O(n + m)
    // -- advancing whichever side is behind, so it never rescans.
    std::vector<int> fast;
    std::set_intersection(a.begin(), a.end(), b.begin(), b.end(),
                          std::back_inserter(fast));

    std::cout << "nested loop:      ";
    for (int x : slow) std::cout << x << ' ';
    std::cout << "\nset_intersection: ";
    for (int x : fast) std::cout << x << ' ';
    std::cout << '\n';
    return 0;
}
