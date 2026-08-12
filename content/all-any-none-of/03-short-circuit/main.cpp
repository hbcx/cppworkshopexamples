#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> nums{1, 2, 3, 4, 5, 6, 7, 8};

    // These algorithms STOP as soon as the answer is settled: any_of at the first
    // element that satisfies the predicate, all_of at the first that fails. A
    // counter inside the predicate makes the early stop visible.
    int checks = 0;
    bool anyOver3 = std::any_of(nums.begin(), nums.end(), [&checks](int x){
        ++checks;
        return x > 3;
    });
    std::cout << std::boolalpha;
    std::cout << "any > 3 = " << anyOver3 << " after " << checks
              << " checks (of " << nums.size() << ")\n";   // stops at 4 -> 4 checks

    checks = 0;
    bool allOver3 = std::all_of(nums.begin(), nums.end(), [&checks](int x){
        ++checks;
        return x > 3;
    });
    std::cout << "all > 3 = " << allOver3 << " after " << checks
              << " checks\n";   // 1 fails immediately -> 1 check
    return 0;
}
