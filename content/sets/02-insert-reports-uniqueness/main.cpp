#include <iostream>
#include <set>
#include <string>

int main() {
    std::set<std::string> seen;

    const char* stream[] = {"a", "b", "a", "c", "b", "a"};
    int firstTimes = 0;
    for (const char* w : stream) {
        // insert returns pair<iterator,bool>; the bool is true only on first sight,
        // so a set is a natural "have I seen this before?" filter.
        auto result = seen.insert(w);
        if (result.second) {
            ++firstTimes;
            std::cout << "first time seeing " << w << '\n';
        }
    }

    std::cout << "distinct = " << seen.size() << ", firstTimes = " << firstTimes << '\n';

    // count is 0 or 1 for a set (unique keys); contains (C++20) reads clearer.
    std::cout << "count(b) = " << seen.count("b") << '\n';
    return 0;
}
