#include <iostream>
#include <type_traits>

// A scoped enum does not convert to its number on its own, so you convert
// explicitly. static_cast<int>(e) gets the value out; static_cast<Enum>(n) turns
// a number back into an enum. Since C++23 std::to_underlying(e) is the clearer
// spelling of the first direction.

enum class Level : int { Debug = 0, Info = 1, Warning = 2, Error = 3 };

// A portable "to number" that works in any C++ version and never names the
// wrong type -- it reads the enum's own underlying type.
constexpr std::underlying_type<Level>::type toNumber(Level l) {
    return static_cast<std::underlying_type<Level>::type>(l);
}

int main() {
    Level l = Level::Warning;

    // enum -> number
    std::cout << "Warning as number: " << static_cast<int>(l) << "\n";  // 2
    std::cout << "via helper:        " << toNumber(l) << "\n";          // 2

    // number -> enum (you are responsible for the value being valid)
    Level fromInt = static_cast<Level>(3);
    std::cout << "3 as Level == Error? " << (fromInt == Level::Error) << "\n"; // 1

    // C++23: std::to_underlying(l) does the enum -> number direction with no
    // hand-written cast and no chance of naming the wrong type. Shown as a
    // comment so this example stays C++11:
    //     #include <utility>
    //     auto n = std::to_underlying(l);   // same value as static_cast<int>(l)
    return 0;
}
