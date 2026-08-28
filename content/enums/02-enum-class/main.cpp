#include <iostream>

// A scoped enum (enum class) is the modern default. Its enumerators live inside
// the enum's name (Color::Red, not Red), and it does NOT implicitly convert to
// int, so the compiler catches accidental mixing. You opt into a number with an
// explicit cast.

enum class Color { Red, Green, Blue };
enum class Shape { Circle, Square, Triangle };

int main() {
    Color c = Color::Green;

    // c is a Color, not an int. To print its number, cast on purpose:
    std::cout << "Green as number: " << static_cast<int>(c) << "\n";   // 1

    // Names are scoped, so two enums can share an enumerator name with no clash:
    Shape s = Shape::Square;
    std::cout << "Square as number: " << static_cast<int>(s) << "\n";  // 1

    // Comparisons within the same enum work:
    std::cout << "c == Color::Green? " << (c == Color::Green) << "\n"; // 1

    //  int n = c;         // error: no implicit conversion to int
    //  if (c == s) {}     // error: Color and Shape are unrelated types
    //  if (c == 1) {}     // error: cannot compare Color to int
    std::cout << "(mixing Color with int or Shape is a compile error)\n";
    return 0;
}
