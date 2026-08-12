#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> nums{5, 1, 2, 3, 4};

    // BAD: using any_of's predicate to ALSO do per-element work -- here to count
    // the positives while checking "is any over 3". Because any_of stops at the
    // first match, the side effect does not run for every element.
    int positives = 0;
    bool anyOver3 = std::any_of(nums.begin(), nums.end(), [&positives](int x){
        if (x > 0) ++positives;   // meant to see every element -- but it will not
        return x > 3;
    });
    // The first element (5) already satisfies x > 3, so the scan stops there and
    // the predicate ran once: positives is 1, not 5.
    std::cout << "any over 3? " << std::boolalpha << anyOver3
              << " -- positives counted = " << positives << " (expected 5)\n";

    // FIX: do not hide a per-element job in a quantifier's predicate. Count with
    // the algorithm meant for it, and ask the yes/no question separately.
    positives = static_cast<int>(
        std::count_if(nums.begin(), nums.end(), [](int x){ return x > 0; }));
    anyOver3 = std::any_of(nums.begin(), nums.end(), [](int x){ return x > 3; });
    std::cout << "fixed: positives = " << positives
              << ", any over 3 = " << anyOver3 << '\n';
    return 0;
}
