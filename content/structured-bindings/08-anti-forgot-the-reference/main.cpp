#include <iostream>
#include <string>
#include <vector>

// Anti-pattern: binding by value when you meant to modify or to avoid a copy.
// auto [a, b] copies the whole source object first, then binds to the copy. In a
// loop over big elements that is a silent copy per iteration; and writing
// through the names changes the copy, not the original.

struct Record {
    std::string label;
    int         value;
};

int main() {
    std::vector<Record> records{{"a", 1}, {"b", 2}, {"c", 3}};

    // BUG: auto [..] copies each Record; the increment is written to the copy.
    for (auto [label, value] : records) {
        value += 100;           // modifies the per-iteration COPY
    }
    std::cout << "after value-binding loop, records[0].value = "
              << records[0].value << "\n";    // still 1 -- nothing changed

    // FIX: bind by reference to touch the real elements (and skip the copy).
    for (auto& [label, value] : records) {
        value += 100;
    }
    std::cout << "after reference loop, records[0].value = "
              << records[0].value << "\n";     // 101

    // For read-only access over big elements, const auto& avoids the copy too.
    long total = 0;
    for (const auto& [label, value] : records) {
        total += value;
    }
    std::cout << "total = " << total << "\n";  // 101 + 102 + 103 = 306
    return 0;
}
