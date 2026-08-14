#include <algorithm>
#include <iostream>
#include <vector>

static void print(const char* label, const std::vector<int>& v) {
    std::cout << label;
    for (int x : v) std::cout << ' ' << x;
    std::cout << '\n';
}

int main() {
    // A dataset where -1 is used as a "missing reading" sentinel.
    std::vector<int> readings{20, -1, 22, -1, 25, 21};
    print("before:", readings);

    // std::replace(first, last, old, new) overwrites every element equal to old
    // with new, in place. Here: turn every -1 sentinel into 0. It touches only
    // the matching elements and never changes the size of the range.
    std::replace(readings.begin(), readings.end(), -1, 0);
    print("after: ", readings);

    // replace returns nothing -- it mutates the range you pass. All occurrences
    // are replaced in a single pass; if the value is absent, nothing happens.
    return 0;
}
