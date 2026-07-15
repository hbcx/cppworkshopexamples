#include <algorithm>
#include <iostream>
#include <iterator>
#include <set>

static void print(const char* label, const std::set<int>& s) {
    std::cout << label;
    for (int x : s) {
        std::cout << ' ' << x;
    }
    std::cout << '\n';
}

int main() {
    std::set<int> a{1, 2, 3, 4, 5};
    std::set<int> b{3, 4, 5, 6, 7};

    // std::set is always sorted, which is exactly the precondition the <algorithm>
    // set operations require. Each result is collected into a fresh set.
    std::set<int> inter;
    std::set<int> uni;
    std::set<int> diff;
    std::set_intersection(a.begin(), a.end(), b.begin(), b.end(),
                          std::inserter(inter, inter.end()));
    std::set_union(a.begin(), a.end(), b.begin(), b.end(),
                   std::inserter(uni, uni.end()));
    std::set_difference(a.begin(), a.end(), b.begin(), b.end(),
                        std::inserter(diff, diff.end()));

    print("intersection (a & b):", inter);
    print("union       (a | b):", uni);
    print("difference  (a - b):", diff);
    return 0;
}
