#include <iostream>
#include <utility>

enum class Color : unsigned char { Red = 1, Green = 2, Blue = 4 };

int main() {
    Color c = Color::Green;

    // std::to_underlying(e) (C++23) converts a scoped enum to its underlying
    // integer with no hand-written cast, and it can never name the wrong type --
    // it reads the enum's own underlying type. It is exactly
    // static_cast<std::underlying_type_t<Color>>(e), only clearer and safer.
    auto n = std::to_underlying(c);   // an unsigned char with value 2

    std::cout << "Green underlying = " << static_cast<int>(n) << "\n";
    std::cout << "Blue underlying  = " << static_cast<int>(std::to_underlying(Color::Blue)) << "\n";
    return 0;
}
