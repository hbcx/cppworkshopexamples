#include <chrono>
#include <iostream>

// Anti-pattern: the unit lives only in the name and a comment. Returns the span
// it would wait, in milliseconds.
static long waitInt(int timeout_ms) { return timeout_ms; }

// Fixed: the unit is in the TYPE. Any duration converts to the common unit.
static std::chrono::milliseconds waitDur(std::chrono::milliseconds timeout) { return timeout; }

int main() {
    using namespace std::chrono;

    // A caller who thinks in seconds passes 5 -- and silently gets a 5 ms wait,
    // a 1000x error no compiler can see.
    long got = waitInt(5);                       // caller meant 5 SECONDS
    std::cout << "[buggy] int API told 5 (meant seconds), will wait " << got << " ms\n";

    // With a duration parameter, seconds(5) converts to 5000 ms automatically,
    // and the unit cannot be omitted.
    milliseconds dur = waitDur(seconds(5));
    std::cout << "[correct] duration API given seconds(5), will wait " << dur.count() << " ms\n";
    // waitDur(5);  // does not compile: an int is not a duration -- unit required
    return 0;
}
