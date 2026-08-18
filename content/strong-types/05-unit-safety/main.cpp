#include <iostream>

// A strong type does not have to wrap only ids. Give each physical unit its own
// type and the compiler tracks dimensions for you: meters cannot be added to
// seconds, and dividing one by the other produces a THIRD unit -- a speed.
template <typename T, typename Tag>
struct Unit {
    T value;
    explicit constexpr Unit(T v) : value(v) {}
};

struct MeterTag;
struct SecondTag;
struct SpeedTag;
using Meters = Unit<double, MeterTag>;
using Seconds = Unit<double, SecondTag>;
using MetersPerSecond = Unit<double, SpeedTag>;

// Adding two lengths stays a length.
constexpr Meters operator+(Meters a, Meters b) { return Meters{a.value + b.value}; }

// Dividing a distance by a time changes the unit: meters / seconds -> m/s.
// The result type says so, so a speed can never be confused with a bare double
// or with a distance, and meters + seconds does not compile at all.
constexpr MetersPerSecond operator/(Meters d, Seconds t) {
    return MetersPerSecond{d.value / t.value};
}

int main() {
    constexpr Meters distance{100.0};
    constexpr Seconds time{10.0};

    constexpr MetersPerSecond speed = distance / time;   // computed at compile time
    static_assert(speed.value == 10.0, "100 m over 10 s is 10 m/s");

    Meters total = distance + Meters{50.0};

    std::cout << "speed = " << speed.value << " m/s\n";
    std::cout << "total distance = " << total.value << " m\n";

    // Meters bad = distance + time;   // COMPILE ERROR: no operator+(Meters, Seconds)
    // double raw = distance;          // COMPILE ERROR: explicit, no implicit decay
    return 0;
}
