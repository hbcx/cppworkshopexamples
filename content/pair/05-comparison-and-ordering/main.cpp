#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

int main() {
    // pair comparison is lexicographic: compare .first, and only on a tie
    // fall back to .second. All six relational operators work this way.
    std::pair<int, int> a{1, 9};
    std::pair<int, int> b{2, 0};
    std::cout << "(1,9) < (2,0)? " << (a < b ? "yes" : "no") << '\n';   // yes: 1 < 2

    std::pair<int, int> c{1, 9};
    std::pair<int, int> d{1, 5};
    std::cout << "(1,9) < (1,5)? " << (c < d ? "yes" : "no") << '\n';   // no: tie, 9 > 5

    // So a pair is a ready-made composite sort key: sort by age, then by name
    // for ties -- no custom comparator needed.
    std::vector<std::pair<int, std::string>> people{
        {41, "bea"}, {36, "ada"}, {41, "abe"}, {36, "zed"}};
    std::sort(people.begin(), people.end());
    for (const auto& p : people) {
        std::cout << p.first << ' ' << p.second << '\n';
    }
    return 0;
}
