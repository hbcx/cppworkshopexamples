#include <algorithm>
#include <vector>
#include <string>
#include <iostream>

struct Row {
    std::string name;
    int group;
};

int main() {
    std::vector<Row> rows{
        {"first", 1},
        {"second", 1},
        {"third", 1},
    };

    // BAD ASSUMPTION: std::sort does NOT guarantee it keeps the original order of
    // elements that compare equal. Sorting these by group -- all group 1, so all
    // equal -- may leave them in any order; you cannot rely on first, second, third.
    std::sort(rows.begin(), rows.end(),
        [](const Row& a, const Row& b){ return a.group < b.group; });
    // (We do not PRINT this result: it is unspecified, so it could differ between
    // library implementations. That unpredictability is exactly the bug.)

    // GOOD: when the order of equal elements must be preserved, stable_sort
    // guarantees it -- the output stays first, second, third.
    std::vector<Row> stable{
        {"first", 1}, {"second", 1}, {"third", 1},
    };
    std::stable_sort(stable.begin(), stable.end(),
        [](const Row& a, const Row& b){ return a.group < b.group; });
    std::cout << "stable_sort keeps input order of equal keys:\n";
    for (const auto& r : stable) std::cout << "  " << r.name << '\n';
    return 0;
}
