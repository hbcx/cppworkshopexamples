#include <iostream>

// C++20 'using enum' brings a scoped enum's enumerators into the current scope,
// so you can write Red instead of Color::Red where the context is clear -- most
// useful inside a switch, where every label would otherwise repeat the enum name.

enum class Color { Red, Green, Blue };

const char* hex(Color c) {
    switch (c) {
        using enum Color;        // Red/Green/Blue usable unqualified below
        case Red:   return "#FF0000";
        case Green: return "#00FF00";
        case Blue:  return "#0000FF";
    }
    return "#000000";
}

int main() {
    // A using-enum declaration can also appear in a block scope.
    using enum Color;
    Color c = Green;             // no Color:: needed here
    std::cout << "Green -> " << hex(c) << "\n";
    std::cout << "Blue  -> " << hex(Blue) << "\n";
    return 0;
}
