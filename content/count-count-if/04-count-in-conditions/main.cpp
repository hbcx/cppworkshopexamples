#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> tasks{1, 0, 1, 1, 0, 1};   // 1 = done, 0 = not done

    // Count how many are done, then use the number in a summary and in conditions.
    auto done = std::count(tasks.begin(), tasks.end(), 1);
    auto total = static_cast<long>(tasks.size());
    std::cout << "progress: " << done << " / " << total << '\n';   // 4 / 6

    // "at least N": a quorum or threshold check.
    bool quorum = done >= 3;
    std::cout << "at least 3 done? " << std::boolalpha << quorum << '\n';   // true

    // "the majority": compare against half without dividing (avoids rounding).
    bool majority = done * 2 > total;
    std::cout << "majority done? " << majority << '\n';   // true (4*2 = 8 > 6)
    return 0;
}
