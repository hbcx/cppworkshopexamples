#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> nums{2, 4, 5, 6};

    // BAD: roundabout ways to ask "are all even?" and "is any odd?" -- each is less
    // clear than the quantifier, and some do more work than needed.

    // (1) a hand-rolled loop with a flag
    bool allEvenLoop = true;
    for (int x : nums)
        if (x % 2 != 0) { allEvenLoop = false; break; }

    // (2) count_if compared to size -- this counts EVERY element, no short-circuit
    bool allEvenCount =
        std::count_if(nums.begin(), nums.end(), [](int x){ return x % 2 == 0; })
        == static_cast<long>(nums.size());

    // (3) find_if != end() used only to mean "any odd exists"
    bool anyOddFind =
        std::find_if(nums.begin(), nums.end(), [](int x){ return x % 2 != 0; }) != nums.end();

    // GOOD: say exactly what you mean, and get the short-circuit for free.
    bool allEven = std::all_of(nums.begin(), nums.end(), [](int x){ return x % 2 == 0; });
    bool anyOdd  = std::any_of(nums.begin(), nums.end(), [](int x){ return x % 2 != 0; });

    std::cout << std::boolalpha;
    std::cout << "all even (loop / count / all_of): "
              << allEvenLoop << ' ' << allEvenCount << ' ' << allEven << '\n';
    std::cout << "any odd  (find / any_of):         "
              << anyOddFind << ' ' << anyOdd << '\n';
    return 0;
}
