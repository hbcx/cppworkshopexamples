#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> scores{55, 90, 72, 88};

    // The confusion: std::max is the larger of its ARGUMENTS (values), while
    // std::max_element finds the largest in a RANGE and returns an ITERATOR.

    // BAD: std::max(scores.front(), scores.back()) only compares two elements,
    // not the whole vector -- it misses 90 entirely.
    std::cout << "max of first and last only: "
              << std::max(scores.front(), scores.back()) << '\n';   // 88, not the real max

    // Also BAD (shown as a comment): max_element returns an ITERATOR, so treating
    // it as the value does not even compile -- you cannot stream it or compare it
    // to an int. You must dereference it.
    //     if (std::max_element(scores.begin(), scores.end()) > 80) { ... }  // type error

    // GOOD: to get the maximum VALUE of a container, dereference max_element.
    auto it = std::max_element(scores.begin(), scores.end());
    std::cout << "max of the whole vector:    " << *it << '\n';     // 90
    return 0;
}
