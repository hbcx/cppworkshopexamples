#include <iostream>
#include <compare>
#include <concepts>

// Anti-pattern: a hand-written <=> that returns `auto` while its branches produce
// different ordering categories. Comparing an int gives strong_ordering, a double
// gives partial_ordering, and `auto` cannot be both.

//  BAD (does not compile -- shown as a comment):
//    auto operator<=>(const Reading& o) const {
//        if (auto c = id <=> o.id; c != 0) return c;   // strong_ordering
//        return value <=> o.value;                     // partial_ordering
//    }                                                 // auto: inconsistent deduction

struct Reading {
    int id;
    double value;

    // FIX: name the common category. strong_ordering (from id) converts to it.
    std::partial_ordering operator<=>(const Reading& other) const {
        if (auto c = id <=> other.id; c != 0) return c;
        return value <=> other.value;
    }
    bool operator==(const Reading& other) const {
        return id == other.id && value == other.value;
    }
};

// A defaulted <=> over the same members deduces the common category on its own:
struct AutoReading {
    int id;
    double value;
    auto operator<=>(const AutoReading&) const = default;
};

int main() {
    static_assert(std::same_as<decltype(AutoReading{} <=> AutoReading{}),
                               std::partial_ordering>);

    Reading a{1, 2.5};
    Reading b{1, 9.0};
    Reading c{2, 0.0};
    Reading d{1, 2.5};

    std::cout << "a < b? " << (a < b) << "\n";   // 1  (same id, 2.5 < 9.0)
    std::cout << "a < c? " << (a < c) << "\n";   // 1  (id 1 < 2)
    std::cout << "a == d? " << (a == d) << "\n"; // 1
    return 0;
}
