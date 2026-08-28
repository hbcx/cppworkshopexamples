#include <iostream>

// A temperature type that converts to and from a plain number implicitly, to show
// both directions of user-defined conversion next to the built-in ones.

class Celsius {
public:
    Celsius(double degrees) : degrees_(degrees) {}   // converting ctor: double -> Celsius
    operator double() const { return degrees_; }     // conversion operator: Celsius -> double
private:
    double degrees_;
};

double asFahrenheit(double celsius) { return celsius * 9.0 / 5.0 + 32.0; }

int main() {
    // Standard conversion: int widens to double with no cast.
    double d = 7;
    std::cout << "int -> double: " << d << "\n";                 // 7

    // User-defined, double -> Celsius (the converting constructor):
    Celsius body = 36.6;
    // User-defined, Celsius -> double (the conversion operator), then int->double
    // for the argument:
    std::cout << "36.6 C = " << asFahrenheit(body) << " F\n";    // 97.88

    // bool -> int is a standard conversion too:
    int flag = (2 < 3);
    std::cout << "bool -> int: " << flag << "\n";                // 1
    return 0;
}
