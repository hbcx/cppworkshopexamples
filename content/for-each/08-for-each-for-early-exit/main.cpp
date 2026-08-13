#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v{3, 8, 15, 6, 20, 4};

    // BAD: for_each cannot stop early -- it always visits every element. Emulating
    // "find the first over 10" with a captured flag keeps the loop running after
    // the answer is found, and the control flow is awkward.
    int firstBig = -1;
    std::for_each(v.begin(), v.end(), [&firstBig](int x) {
        if (firstBig == -1 && x > 10) firstBig = x;   // still visits the rest
    });
    std::cout << "for_each found: " << firstBig << '\n';

    // GOOD: find_if stops at the first match and returns its position; any_of
    // answers yes/no, both short-circuiting instead of scanning the whole range.
    auto it = std::find_if(v.begin(), v.end(), [](int x) { return x > 10; });
    std::cout << "find_if found: " << (it != v.end() ? *it : -1) << '\n';
    bool anyBig = std::any_of(v.begin(), v.end(), [](int x) { return x > 10; });
    std::cout << "any over 10? " << (anyBig ? "yes" : "no") << '\n';
    return 0;
}
