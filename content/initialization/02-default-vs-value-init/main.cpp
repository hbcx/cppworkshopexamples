#include <iostream>
#include <string>

// "Default initialization" and "value initialization" look similar but differ
// exactly where it bites: a built-in left default-initialized has an
// INDETERMINATE value, while value initialization zeroes it.

struct Bag {
    int count;          // a built-in member
    std::string label;  // a class member with its own default constructor
};

int main() {
    // Value initialization with {}: built-in members are zeroed, class members
    // run their default constructor. Everything here is well defined.
    Bag valueInit{};
    std::cout << "value-init count: " << valueInit.count << "\n";       // 0
    std::cout << "value-init label: \"" << valueInit.label << "\"\n";   // ""

    // T() and T{} on a built-in also value-initialize to zero.
    std::cout << "int():    " << int() << "\n";      // 0
    std::cout << "int{}:    " << int{} << "\n";      // 0
    std::cout << "double{}: " << double{} << "\n";   // 0

    // Default initialization of a LOCAL built-in leaves it indeterminate:
    //
    //     Bag defaultInit;                 // count is a garbage value
    //     std::cout << defaultInit.count;  // undefined behaviour -- do NOT read it
    //
    // The std::string member would still be "" (it has a default constructor),
    // but the int member would be garbage. We do not run that.
    //
    // The rule: for a built-in, prefer value initialization ({}) so the value is
    // always defined. A bare "int x;" as a local is the one to watch.
    int defined{};
    std::cout << "always-defined int{}: " << defined << "\n";           // 0
    return 0;
}
