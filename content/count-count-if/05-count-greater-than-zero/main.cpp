#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> nums{7, 2, 4, 8, 10, 12, 14, 16};   // one odd, at the front

    // BAD: count scans the WHOLE range to produce a number, then you throw the
    // number away and only ask was it more than zero. Existence does not need a
    // full count. A call counter in the predicate shows the wasted work.
    int checks = 0;
    bool hasOddBad = std::count_if(nums.begin(), nums.end(), [&checks](int x){
        ++checks;
        return x % 2 == 1;
    }) > 0;
    std::cout << std::boolalpha;
    std::cout << "any odd (count > 0)? " << hasOddBad
              << " after " << checks << " checks (of " << nums.size() << ")\n";

    // GOOD: any_of answers the same yes/no question and STOPS at the first match.
    checks = 0;
    bool hasOddGood = std::any_of(nums.begin(), nums.end(), [&checks](int x){
        ++checks;
        return x % 2 == 1;
    });
    std::cout << "any odd (any_of)?   " << hasOddGood
              << " after " << checks << " checks\n";
    return 0;
}
