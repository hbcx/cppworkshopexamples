#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

int main() {
    // Anti-pattern: a long chain of sticky manipulators to format one line. It
    // is verbose, order-dependent, easy to get wrong, and leaks state unless you
    // restore it. For a fixed template, std::format (C++20) says the same thing
    // in one string -- but on C++11/17 the tidy stream version keeps the
    // formatting LOCAL to a stringstream so it cannot leak onto std::cout.
    int id = 42;
    double amount = 3.5;

    // Manipulator soup on a throwaway stringstream (std::cout's state untouched):
    std::ostringstream line;
    line << "#" << std::setw(4) << std::setfill('0') << id
         << " $" << std::fixed << std::setprecision(2) << amount;
    std::cout << line.str() << '\n';   // #0042 $3.50

    // The C++20 equivalent, for comparison (not compiled here):
    //   std::format("#{:04d} ${:.2f}", id, amount)  ->  #0042 $3.50
    std::cout << "keep sticky formatting inside a local stream, or use std::format\n";
    return 0;
}
