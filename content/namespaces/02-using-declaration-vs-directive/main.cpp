#include <iostream>

namespace lib {
    int value() { return 42; }
    const char* name() { return "lib"; }
}

int main() {
    {
        using lib::value;    // using-DECLARATION: brings exactly ONE name into scope
        std::cout << "value() = " << value() << "\n";     // unqualified, resolves to lib::value
        std::cout << "name()  = " << lib::name() << "\n"; // name() still needs the qualifier
    }
    {
        using namespace lib; // using-DIRECTIVE: makes ALL of lib's names available here
        std::cout << "value() = " << value() << ", name() = " << name() << "\n";
    }
    // Both were limited to their block; outside, qualify again.
    std::cout << "qualified: " << lib::value() << "\n";
    return 0;
}
