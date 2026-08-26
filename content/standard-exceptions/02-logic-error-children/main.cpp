// The logic_error children, each naming a specific kind of precondition bug:
//   invalid_argument -- an argument's value is not allowed
//   domain_error     -- input outside a function's mathematical domain
//   length_error     -- an object asked to exceed its maximum size
//   out_of_range     -- an index or key outside the valid range
// The standard library throws these too (vector::at -> out_of_range,
// std::stoi -> invalid_argument, an over-long string -> length_error).
#include <iostream>
#include <stdexcept>
#include <cmath>

// A domain check: the square root is only defined for non-negative input.
double checked_sqrt(double x) {
    if (x < 0) throw std::domain_error("sqrt of a negative number");
    return std::sqrt(x);
}

int main() {
    // Throw one of each and report through the shared logic_error base.
    try { throw std::invalid_argument("color name not recognized"); }
    catch (const std::logic_error& e) { std::cout << "invalid_argument: " << e.what() << '\n'; }

    try { (void)checked_sqrt(-1.0); }
    catch (const std::logic_error& e) { std::cout << "domain_error:     " << e.what() << '\n'; }

    try { throw std::length_error("string longer than max_size"); }
    catch (const std::logic_error& e) { std::cout << "length_error:     " << e.what() << '\n'; }

    try { throw std::out_of_range("row 10 of 3"); }
    catch (const std::logic_error& e) { std::cout << "out_of_range:     " << e.what() << '\n'; }

    return 0;
}
