#include <iostream>

// constinit guarantees a variable is CONSTANT-INITIALIZED -- its value is set at
// compile time, before any dynamic initialization runs -- which avoids the static
// initialization order problem between translation units. Unlike constexpr, a
// constinit variable is NOT const: it can change afterwards.
constexpr int configFor(int base) { return base * 10 + 5; }

constinit int gCounter = configFor(4);   // 45, initialized at compile time

int main() {
    std::cout << "start: " << gCounter << "\n";   // 45
    gCounter += 100;                               // allowed: constinit is not const
    std::cout << "after: " << gCounter << "\n";    // 145

    // constinit only guarantees the INITIALIZER is a constant expression. If
    // configFor were not constexpr, the declaration above would be a COMPILE
    // error -- which is the point: it catches a global that would otherwise be
    // dynamically initialized in an order no standard defines.
    std::cout << "done\n";
}
