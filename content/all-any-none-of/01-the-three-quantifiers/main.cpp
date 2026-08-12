#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> nums{2, 4, 6, 8, 10};

    // Each takes a range (an iterator pair) and a predicate, and returns a bool.
    bool allEven  = std::all_of(nums.begin(), nums.end(), [](int x){ return x % 2 == 0; });
    bool anyOver5 = std::any_of(nums.begin(), nums.end(), [](int x){ return x > 5; });
    bool noneNeg  = std::none_of(nums.begin(), nums.end(), [](int x){ return x < 0; });

    std::cout << std::boolalpha;
    std::cout << "all even?      " << allEven  << '\n';   // true
    std::cout << "any over 5?    " << anyOver5 << '\n';   // true
    std::cout << "none negative? " << noneNeg  << '\n';   // true

    // none_of is the logical opposite of any_of over the same predicate.
    bool anyOdd = std::any_of(nums.begin(), nums.end(), [](int x){ return x % 2 == 1; });
    std::cout << "any odd?       " << anyOdd << " (so none_of odd is " << !anyOdd << ")\n";
    return 0;
}
