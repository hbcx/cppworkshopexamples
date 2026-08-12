#include <algorithm>
#include <vector>
#include <string>
#include <iostream>

int main() {
    std::vector<std::string> log{
        "ok", "ok", "error", "ok", "warn", "error", "ok",
    };

    // Build a small tally for a report by counting each category. For a fixed,
    // known set of categories this is clear and needs no frequency map.
    for (const char* level : {"ok", "warn", "error"}) {
        auto n = std::count(log.begin(), log.end(), level);
        std::cout << level << ": " << n << '\n';
    }
    return 0;
}
