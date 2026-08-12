#include <algorithm>
#include <vector>
#include <string>
#include <iostream>

int main() {
    std::vector<std::string> users{"ada", "linus", "grace"};

    // BAD: search once to check presence, then search AGAIN to use the result --
    // two full scans for a single lookup.
    if (std::find(users.begin(), users.end(), "grace") != users.end()) {
        auto again = std::find(users.begin(), users.end(), "grace");   // second scan
        std::cout << "bad:  found " << *again << " (searched twice)\n";
    }

    // GOOD: search once, keep the iterator, check it, then use it.
    auto it = std::find(users.begin(), users.end(), "grace");
    if (it != users.end())
        std::cout << "good: found " << *it << " (searched once)\n";
    return 0;
}
