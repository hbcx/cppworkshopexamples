#include <iostream>
#include <string>

struct Printer {
    void print(int n) const         { std::cout << "int " << n << "\n"; }
    void print(std::string s) const { std::cout << "str " << s << "\n"; }
};

int main() {
    Printer p;

    // BAD (described, not compiled): print is OVERLOADED, so a bare &Printer::print
    // has no single type -- decltype cannot pick one:
    //   using PtrBad = decltype(&Printer::print);   // ERROR: which print?

    // GOOD: name the overload with a static_cast to its exact signature. Then the
    // type (and the pointer) is unambiguous.
    using PrintInt = void (Printer::*)(int) const;
    PrintInt pi = static_cast<PrintInt>(&Printer::print);   // selects print(int)
    (p.*pi)(7);

    using PrintStr = void (Printer::*)(std::string) const;
    PrintStr ps = static_cast<PrintStr>(&Printer::print);   // selects print(std::string)
    (p.*ps)(std::string("hello"));

    std::cout << "done\n";
    return 0;
}
