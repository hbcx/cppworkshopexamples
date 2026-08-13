#include <algorithm>
#include <ranges>
#include <string>
#include <vector>
#include <iostream>

struct Row {
    int userId;
    std::string action;
};

int main() {
    std::vector<int> v{1, 1, 2, 3, 3, 3, 2};

    // ranges::unique returns a SUBRANGE marking the leftover tail {new_end, end},
    // so you hand it straight to erase without spelling out end() a second time.
    auto tail = std::ranges::unique(v);
    v.erase(tail.begin(), tail.end());
    std::cout << "deduped consecutive: ";
    for (int x : v) std::cout << x << ' ';
    std::cout << '\n';

    // A projection collapses neighbours equal in one FIELD: rows from the same
    // user in a row become one, comparing by userId while carrying the whole row.
    std::vector<Row> rows{
        {1, "open"}, {1, "scroll"}, {2, "open"}, {2, "close"}, {1, "open"}};
    auto rtail = std::ranges::unique(rows, {}, &Row::userId);
    rows.erase(rtail.begin(), rtail.end());
    std::cout << "one row per user-run: ";
    for (const auto& r : rows) std::cout << r.userId << ':' << r.action << ' ';
    std::cout << '\n';
    return 0;
}
