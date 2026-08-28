#include <iostream>

// A temperature reading that may or may not have a valid value. We give it two
// conversions: an explicit operator bool (is the reading valid?) and an explicit
// operator double (the value in Celsius). explicit keeps both from firing where
// we did not intend.

class Temperature {
public:
    Temperature() : celsius_(0.0), valid_(false) {}
    explicit Temperature(double c) : celsius_(c), valid_(true) {}

    // Usable in a boolean context -- if (t), while (t) -- but not silently mixed
    // into arithmetic, because it is explicit.
    explicit operator bool() const { return valid_; }

    // Ask for the number on purpose with static_cast<double>(t); explicit stops
    // t from decaying to double in an accidental expression.
    explicit operator double() const { return celsius_; }

private:
    double celsius_;
    bool valid_;
};

int main() {
    Temperature reading{21.5};
    Temperature missing;

    // Direct boolean use is allowed even though the operator is explicit:
    if (reading)
        std::cout << "reading is valid\n";              // printed
    if (!missing)
        std::cout << "missing has no value\n";          // printed

    // Getting the number is a deliberate, visible cast:
    double c = static_cast<double>(reading);
    std::cout << "value = " << c << " C\n";             // 21.5

    // Because both are explicit, a line like `reading + 1` or `reading == missing`
    // does NOT compile -- the surprising conversions are blocked at the source.
    return 0;
}
