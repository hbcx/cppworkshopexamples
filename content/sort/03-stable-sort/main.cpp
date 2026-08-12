#include <algorithm>
#include <vector>
#include <string>
#include <iostream>

struct Entry {
    std::string name;
    int priority;
};

int main() {
    // Input order matters here: b and d both have priority 1, with b before d.
    std::vector<Entry> entries{
        {"a", 2},
        {"b", 1},
        {"c", 3},
        {"d", 1},
        {"e", 2},
    };

    // stable_sort keeps the ORIGINAL order of elements that compare equal. Sorting
    // by priority, b and d (both priority 1) stay in their input order b then d,
    // and a and e (both priority 2) stay a then e.
    std::stable_sort(entries.begin(), entries.end(),
        [](const Entry& x, const Entry& y){ return x.priority < y.priority; });

    std::cout << "stable by priority:\n";
    for (const auto& e : entries)
        std::cout << "  " << e.name << " (p" << e.priority << ")\n";
    return 0;
}
