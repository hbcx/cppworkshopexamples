#include <iostream>

// A plain (unscoped) enum defines a set of named integer constants. It is simple
// and it converts freely to int -- which is convenient and, as later examples
// show, the source of its problems. Its enumerator names also leak into the
// enclosing scope.

enum Color { Red, Green, Blue };                   // Red=0, Green=1, Blue=2

enum Priority { Low = 1, Medium = 5, High = 10 };  // explicit values, with gaps

int main() {
    Color c = Green;
    std::cout << "Green as value: " << c << "\n";          // 1 -- converts to int

    // The names are visible with no qualification (they leaked into this scope):
    std::cout << "Blue = " << Blue << "\n";                // 2

    std::cout << "Low=" << Low << " Medium=" << Medium
              << " High=" << High << "\n";                 // 1 5 10

    // Because it converts to int, an enum works directly in arithmetic and as an
    // array index -- handy, but it means the type checking is weak.
    int table[3] = {100, 200, 300};
    std::cout << "table[Blue] = " << table[Blue] << "\n";  // table[2] = 300
    return 0;
}
