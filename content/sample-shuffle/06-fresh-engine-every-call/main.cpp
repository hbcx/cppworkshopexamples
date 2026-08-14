#include <iostream>
#include <random>

// ANTI-PATTERN: build and seed a new engine on every call.
// Seeding with a constant means the engine restarts from the same state each
// time, so it returns the SAME value forever. (Seeding from random_device each
// call instead would avoid the repeat but is slow, and two calls in the same
// clock tick can still collide.) Either way, the engine's state never carries
// from one call to the next -- which is the whole point of a generator.
int badRoll() {
    std::mt19937 engine(42);                 // fresh + same seed every call
    std::uniform_int_distribution<int> dice(1, 6);
    return dice(engine);
}

// FIX: one long-lived engine, reused across calls. Marking it static (or making
// it a member, or passing it in) means each call advances the SAME state, so
// the values actually differ.
int goodRoll() {
    static std::mt19937 engine(42);          // created once, kept between calls
    static std::uniform_int_distribution<int> dice(1, 6);
    return dice(engine);
}

int main() {
    std::cout << "badRoll: ";
    for (int i = 0; i < 6; ++i) std::cout << badRoll() << ' ';
    std::cout << "  <- same number every time\n";

    std::cout << "goodRoll:";
    for (int i = 0; i < 6; ++i) std::cout << ' ' << goodRoll();
    std::cout << "  <- one engine, values advance\n";
    return 0;
}
