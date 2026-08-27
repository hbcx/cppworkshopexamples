#include <iostream>
#include <string>

// An "aggregate" is a class with no user-declared constructors, no private or
// protected non-static data members, no virtual functions, and (before its
// members) public base classes. You initialize it by listing its members in
// braces -- no constructor to write.

struct Point {
    int x;
    int y;
};

struct Segment {
    Point from;         // nested aggregate
    Point to;
    const char* label;
};

// Since C++17 an aggregate may have a public base class. You brace the base's
// members first, then the derived member.
struct Labeled : Point {
    std::string name;
};

int main() {
    // List every member in order.
    Point p{3, 4};
    std::cout << "Point: " << p.x << ", " << p.y << "\n";

    // Nested braces mirror the nested aggregates.
    Segment s{{0, 0}, {3, 4}, "diagonal"};
    std::cout << "Segment " << s.label << ": (" << s.from.x << "," << s.from.y
              << ") -> (" << s.to.x << "," << s.to.y << ")\n";

    // Missing trailing initializers are value-initialized: y becomes 0. Both
    // compilers warn here (-Wmissing-field-initializers) -- a useful nudge that
    // you left a field out; this example turns -Werror off to show the behaviour.
    Point half{5};
    std::cout << "Point half:   " << half.x << ", " << half.y << "\n";   // 5, 0

    // {} value-initializes every member.
    Point origin{};
    std::cout << "Point origin: " << origin.x << ", " << origin.y << "\n"; // 0, 0

    // C++17 aggregate with a base: brace the base's members, then the derived
    // member. The inner braces initialize the Point base subobject.
    Labeled l{{1, 2}, "corner"};
    // (C++20 lets you name the members instead -- see designated initializers.)
    std::cout << "Labeled " << l.name << ": " << l.x << ", " << l.y << "\n";
    return 0;
}
