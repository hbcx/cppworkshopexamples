#include <algorithm>
#include <vector>
#include <functional>
#include <iostream>

int main() {
    std::vector<int> scores{7, 2, 9, 4, 1, 8, 5, 3, 6};

    // BAD: a full O(n log n) sort just to read the largest value. You ordered the
    // whole range and used only one element of it.
    std::vector<int> a = scores;
    std::sort(a.begin(), a.end());
    int maxViaSort = a.back();
    std::cout << "max via full sort:   " << maxViaSort << '\n';   // 9

    // GOOD: max_element is one O(n) pass for the largest (min_element for smallest).
    int maxFast = *std::max_element(scores.begin(), scores.end());
    std::cout << "max via max_element: " << maxFast << '\n';      // 9

    // BAD: a full sort to read only the top few. GOOD: partial_sort orders just
    // those, cheaper than sorting everything.
    std::vector<int> b = scores;
    std::partial_sort(b.begin(), b.begin() + 3, b.end(), std::greater<int>{});
    std::cout << "top 3 via partial_sort: " << b[0] << ' ' << b[1] << ' ' << b[2] << '\n';   // 9 8 7
    return 0;
}
