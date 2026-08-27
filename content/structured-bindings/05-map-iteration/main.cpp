#include <iostream>
#include <map>
#include <string>

// The most common use of structured bindings: iterating an associative
// container. Each element is a pair<const Key, Value>, so [key, value] reads far
// better than it->first and it->second.

int main() {
    std::map<std::string, int> scores{
        {"alice", 90}, {"bob", 75}, {"carol", 88}
    };

    // const auto& : no copy of each pair, read-only.
    std::cout << "scores:\n";
    for (const auto& [name, score] : scores) {
        std::cout << "  " << name << " = " << score << "\n";
    }

    // auto& : modify the values in place. The key stays const.
    for (auto& [name, score] : scores) {
        score += 5;
    }

    std::cout << "after +5:\n";
    for (const auto& [name, score] : scores) {
        std::cout << "  " << name << " = " << score << "\n";
    }
    return 0;
}
