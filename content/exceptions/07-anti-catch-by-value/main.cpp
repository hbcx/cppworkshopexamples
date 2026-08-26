// Anti-pattern: catching an exception BY VALUE. The handler copies the thrown
// object into a base-class variable, which SLICES it -- the derived part is
// gone, and a virtual what() no longer reaches the override. Catch by const
// reference instead, so the full dynamic type survives.
#include <iostream>
#include <stdexcept>

// A specific error that overrides what() with a detailed message.
struct LoadError : std::runtime_error {
    LoadError() : std::runtime_error("generic load failure") {}
    const char* what() const noexcept override {
        return "detailed: widget 42 failed to load";
    }
};

int main() {
    // WRONG: catch by value as the base type. The LoadError is sliced down to a
    // std::runtime_error copy; its overridden what() is lost, so we get the base
    // message instead of the detailed one.
    try {
        throw LoadError();
    }
    catch (std::runtime_error e) {              // by value -> slices
        std::cout << "by value:     " << e.what() << '\n';
    }

    // RIGHT: catch by const reference. No copy, no slicing; the virtual what()
    // dispatches to LoadError's override.
    try {
        throw LoadError();
    }
    catch (const std::runtime_error& e) {       // by reference -> full type
        std::cout << "by reference: " << e.what() << '\n';
    }
    return 0;
}
