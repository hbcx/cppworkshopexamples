#include <iostream>
#include <string>

std::string buildGreeting(const std::string& name) {
    return "Hello, " + name + "!";
}

int main() {
    std::string name = "Ada";

    // ANTI-PATTERN: the compiler warned that `greeting` was unused. Instead of
    // asking WHY, the warning was silenced with [[maybe_unused]] -- which hid the
    // real bug: the greeting was built and then never printed.
    //
    //     [[maybe_unused]] std::string greeting = buildGreeting(name);
    //     std::cout << name << "\n";      // oops -- printed the name, not the greeting
    //
    // [[maybe_unused]] is for names that are legitimately unused in some builds,
    // not a mute button for a warning that is pointing at a mistake.

    // FIX: use the value the warning was telling you about.
    std::string greeting = buildGreeting(name);
    std::cout << greeting << "\n";
    return 0;
}
