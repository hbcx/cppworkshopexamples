// Anti-pattern: elements exposing their internals to satisfy visitors.
//
// A visitor works on an element from the outside, so it needs some access to the
// element's data. The trap is letting that need drive the element to publish its
// internal REPRESENTATION -- raw public fields -- so visitors read the storage
// directly. Now every visitor is coupled to how the element stores its state,
// and changing that storage breaks them all. The fix is to expose a minimal,
// stable interface (a query, not a field), so the representation stays private
// and free to change.

#include <iostream>

// ---- BAD: the element publishes its representation for visitors to read ----
struct TemperatureBad {
    double celsius; // public raw field: visitors read the storage directly
};
double toFahrenheitBad(const TemperatureBad& t) {
    return t.celsius * 9.0 / 5.0 + 32.0; // coupled to the celsius representation
}

// ---- GOOD: a stable query; the representation stays private ----
class TemperatureGood {
    double celsius_; // representation is private and free to change later
public:
    explicit TemperatureGood(double c) : celsius_(c) {}
    double celsius() const { return celsius_; } // the minimal interface a visitor needs
};
double toFahrenheitGood(const TemperatureGood& t) {
    return t.celsius() * 9.0 / 5.0 + 32.0; // depends on the query, not the storage
}

int main() {
    TemperatureBad bad{ 100.0 };
    std::cout << "bad:  " << toFahrenheitBad(bad) << " F\n";

    TemperatureGood good(100.0);
    std::cout << "good: " << toFahrenheitGood(good) << " F\n";
    return 0;
}
