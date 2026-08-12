#include <algorithm>
#include <vector>
#include <string>
#include <iterator>
#include <iostream>

int main() {
    // find_if_not returns the first element that does NOT satisfy the predicate --
    // the mirror of find_if. A common use is skipping a leading run.
    std::string text = "   hello";
    auto firstReal = std::find_if_not(text.begin(), text.end(),
        [](char c){ return c == ' '; });
    std::cout << "leading spaces: " << std::distance(text.begin(), firstReal) << '\n';   // 3
    std::cout << "trimmed: '" << std::string(firstReal, text.end()) << "'\n";

    // Validation: find the first entry that is NOT valid (here, not positive).
    std::vector<int> amounts{10, 25, 40, -5, 8};
    auto firstBad = std::find_if_not(amounts.begin(), amounts.end(),
        [](int x){ return x > 0; });
    if (firstBad != amounts.end())
        std::cout << "first invalid amount: " << *firstBad << '\n';   // -5
    return 0;
}
