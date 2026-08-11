#include <iostream>

// A consteval function is an "immediate function": every call MUST be evaluated
// at compile time. That makes it perfect for validating a literal -- a bad value
// fails to COMPILE, never at run time.
consteval int percent(int p) {
    if (p < 0 || p > 100) {
        throw "percent out of range";   // reached during constant evaluation => compile error
    }
    return p;
}

// consteval is often used to build a checked value at the point of definition.
constexpr int kVolume = percent(80);    // ok: checked and folded at compile time

int main() {
    std::cout << "volume = " << kVolume     << "\n";
    std::cout << "half   = " << percent(50) << "\n";   // also compile time, result is 50

    // percent(150);   // COMPILE ERROR: the throw is reached during constant
    //                 // evaluation, so the bad literal never builds. A plain
    //                 // constexpr function would allow this and only throw at
    //                 // run time if it were called at run time.
    //
    // int r = 30;
    // percent(r);     // COMPILE ERROR too: a consteval call needs a constant
    //                 // argument -- it cannot run at run time at all.
    std::cout << "done\n";
}
