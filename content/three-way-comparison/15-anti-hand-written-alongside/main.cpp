#include <iostream>
#include <compare>
#include <string>

// Anti-pattern: a <=> added to a type that still has an old hand-written
// operator<. The two order by different fields, and because a < b uses the old
// operator while b > a is rewritten from <=>, they contradict each other.

struct BadItem {
    int id;
    std::string name;

    // New: order by id.
    std::strong_ordering operator<=>(const BadItem& other) const {
        return id <=> other.id;
    }
    bool operator==(const BadItem& other) const { return id == other.id; }

    // Leftover from before <=>: orders by name. This is the bug.
    bool operator<(const BadItem& other) const { return name < other.name; }
};

struct GoodItem {
    int id;
    std::string name;
    // Only <=> (and ==). One source of truth for the order.
    std::strong_ordering operator<=>(const GoodItem& other) const {
        return id <=> other.id;
    }
    bool operator==(const GoodItem& other) const { return id == other.id; }
};

int main() {
    BadItem a{1, "zebra"};
    BadItem b{2, "apple"};

    // a < b calls the leftover operator< (by name): "zebra" < "apple" is false.
    // b > a is rewritten from <=> (by id): (b <=> a) > 0 is true.
    // These must always agree -- and here they do not.
    std::cout << "bad:  a < b = " << (a < b)
              << ", b > a = " << (b > a) << "  (should match)\n";   // 0, 1

    GoodItem c{1, "zebra"};
    GoodItem d{2, "apple"};
    // Both go through <=> (by id), so they agree.
    std::cout << "good: c < d = " << (c < d)
              << ", d > c = " << (d > c) << "\n";                   // 1, 1
    return 0;
}
