// The basic mechanic: a function reports a failure by THROWING, and the caller
// handles it in a try/catch. Control jumps straight from the throw to the
// matching catch -- every statement in between is skipped.
#include <iostream>
#include <stdexcept>
#include <string>

// Parse a non-negative count. On bad input it cannot return a valid number, so
// it throws instead of returning a sentinel the caller might forget to check.
int parse_count(const std::string& text) {
    int value = std::stoi(text);            // stoi itself throws on non-numbers
    if (value < 0) {
        throw std::runtime_error("count must not be negative: " + text);
    }
    return value;
}

int main() {
    // A throw inside the try transfers control to the catch below.
    try {
        int good = parse_count("7");
        std::cout << "parsed: " << good << '\n';

        int bad = parse_count("-3");        // throws here
        std::cout << "never printed: " << bad << '\n';   // skipped
    }
    // Catch by const reference: no copy, and it also catches derived types.
    catch (const std::runtime_error& e) {
        std::cout << "caught: " << e.what() << '\n';
    }

    // After the catch handled the error, the program keeps running normally.
    std::cout << "still running\n";
    return 0;
}
