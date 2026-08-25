#include <iostream>
#include <sstream>
#include <string>

int main() {
    // Anti-pattern: spinning up a stringstream just to convert ONE value.
    // Constructing the stream builds and imbues a locale, allocates a buffer,
    // and runs the full formatting machinery -- heavy for a single number, and
    // more code than the direct helper.
    int n = 42;

    // Overkill:
    std::ostringstream ss;
    ss << n;
    std::string heavy = ss.str();
    // Simpler and faster for one value:
    std::string light = std::to_string(n);
    std::cout << "same result: " << (heavy == light) << '\n';   // 1

    // Same story parsing. Overkill:
    std::istringstream is("123");
    int parsedHeavy = 0;
    is >> parsedHeavy;
    // Simpler:
    int parsedLight = std::stoi("123");
    std::cout << "same parse: " << (parsedHeavy == parsedLight) << '\n';   // 1

    // Rule: for a single conversion use to_string / stoi (or to_chars /
    // from_chars in C++17). Keep stringstream for SEVERAL values at once.
    std::cout << "prefer to_string/stoi for one value\n";
    return 0;
}
