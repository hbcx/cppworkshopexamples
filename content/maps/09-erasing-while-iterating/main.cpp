#include <iostream>
#include <map>
#include <string>

static void print(const std::map<int, std::string>& m, const char* label) {
    std::cout << label;
    for (const auto& entry : m) {
        std::cout << ' ' << entry.first << ':' << entry.second;
    }
    std::cout << "   (size=" << m.size() << ")\n";
}

int main() {
    // Goal in every block: erase every entry with an even key.

    // Anti-pattern (shown, NOT run): erase(it) invalidates it, so ++it afterwards
    // is undefined behaviour:
    //   for (auto it = m.begin(); it != m.end(); ++it)
    //       if (it->first % 2 == 0) m.erase(it);   // UB: ++it on a dead iterator

    // Fix 1 (C++11): erase returns the iterator to the next element; advance only
    // when you did NOT erase.
    {
        std::map<int, std::string> m{{1, "a"}, {2, "b"}, {3, "c"}, {4, "d"}, {5, "e"}};
        for (auto it = m.begin(); it != m.end(); ) {
            if (it->first % 2 == 0) {
                it = m.erase(it);
            } else {
                ++it;
            }
        }
        print(m, "fix erase-returns-next:");
    }

    // Fix 2 (classic, pre-C++11): the post-increment leaves it on the next element
    // before erase runs on the old one.
    {
        std::map<int, std::string> m{{1, "a"}, {2, "b"}, {3, "c"}, {4, "d"}, {5, "e"}};
        for (auto it = m.begin(); it != m.end(); ) {
            if (it->first % 2 == 0) {
                m.erase(it++);
            } else {
                ++it;
            }
        }
        print(m, "fix erase(it++):       ");
    }
    return 0;
}
