#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{3, 9, 4, 9, 1};

    // A comparator passed to max_element must be a STRICT weak ordering: comp(a, a)
    // must be false. Writing <= makes it reflexive (comp(a, a) is true), which
    // violates the requirement -- the result is then unspecified, and a wrong
    // element can be reported even where a correct comparator exists. Shown only
    // as a comment, because the behavior is not defined:
    //     std::max_element(v.begin(), v.end(),
    //                      [](int a, int b) { return a <= b; });   // <= is not strict

    // GOOD: use a strict less-than (<). max_element and min_element always expect
    // "is a less than b", never "less than or equal".
    auto strict = std::max_element(v.begin(), v.end(),
        [](int a, int b) { return a < b; });
    std::cout << "max with strict <: " << *strict << '\n';

    // For the natural order, pass no comparator at all -- the default uses
    // operator<, which is already a strict weak ordering.
    auto plain = std::max_element(v.begin(), v.end());
    std::cout << "max with default:  " << *plain << '\n';
    return 0;
}
