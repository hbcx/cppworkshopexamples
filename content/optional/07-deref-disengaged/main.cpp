#include <iostream>
#include <optional>
#include <string>

std::optional<int> lookup(const std::string& key) {
    if (key == "port") return 8080;
    return std::nullopt;
}

int main() {
    // Anti-pattern: using * (or ->) on the result of a fail-able call without
    // checking first. On a miss the optional is empty and * reads a value that
    // is not there -- undefined behaviour, exactly like dereferencing nullptr.
    // Not run:
    //
    //     int p = *lookup("missing");   // UB: no value to read
    //
    // It is easy to write because * makes optional look like a plain handle.

    // Fix 1: guard with the bool conversion, then * is safe.
    if (auto p = lookup("port")) {
        std::cout << "port = " << *p << '\n';
    }
    auto miss = lookup("missing");
    if (!miss) std::cout << "missing: handled, not dereferenced\n";

    // Fix 2: value_or folds the miss into a default in one expression.
    std::cout << "value_or: " << lookup("missing").value_or(-1) << '\n';

    // Fix 3: value() turns a wrong assumption into a thrown exception rather
    // than silent UB, for when the value really should be there.
    try {
        std::cout << lookup("missing").value() << '\n';
    } catch (const std::bad_optional_access&) {
        std::cout << "value(): threw instead of corrupting\n";
    }
    return 0;
}
