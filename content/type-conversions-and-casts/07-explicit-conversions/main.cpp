#include <iostream>

// Meters has an EXPLICIT constructor from int, so a bare number never silently
// becomes a Meters -- you must ask for the conversion.
class Meters {
public:
    explicit Meters(int value) : value_(value) {}
    int value() const { return value_; }
    explicit operator bool() const { return value_ != 0; }   // explicit: only in bool contexts
private:
    int value_;
};

void logDistance(const Meters& m) { std::cout << "distance: " << m.value() << " m\n"; }

int main() {
    // Direct construction is fine -- you named the type:
    Meters d{100};
    logDistance(d);                       // distance: 100 m

    // logDistance(100);  // would NOT compile: int does not implicitly become Meters
    // With the conversion asked for by name, it works:
    logDistance(Meters{250});             // distance: 250 m
    logDistance(static_cast<Meters>(3));  // distance: 3 m

    // explicit operator bool: allowed in a condition...
    if (d) std::cout << "d is non-zero\n";            // printed
    // ...but "int x = d;" or "d + 1" do NOT compile, so no accidental arithmetic.
    return 0;
}
