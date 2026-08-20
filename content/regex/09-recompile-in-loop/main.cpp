#include <iostream>
#include <regex>
#include <string>
#include <vector>

// WRONG: the regex is COMPILED on every call -- the pattern is parsed and the
// state machine rebuilt each time, which costs far more than the match itself
// for a short input. In a loop over many inputs this dominates the runtime.
static bool looksLikeIntBad(const std::string& s) {
    std::regex re(R"(^-?\d+$)");            // rebuilt on every single call
    return std::regex_match(s, re);
}

// RIGHT: compile once, reuse. A function-local static builds it on first use and
// keeps it (a namespace-scope or member const std::regex works the same way).
static bool looksLikeIntGood(const std::string& s) {
    static const std::regex re(R"(^-?\d+$)");   // built once, then reused
    return std::regex_match(s, re);
}

int main() {
    std::vector<std::string> inputs = {"42", "-7", "3.14", "x9", "1000"};

    std::cout << std::boolalpha;
    std::cout << "bad (recompiles each call):\n";
    for (const std::string& s : inputs)
        std::cout << "  " << s << " -> " << looksLikeIntBad(s) << '\n';

    std::cout << "good (compiled once):\n";
    for (const std::string& s : inputs)
        std::cout << "  " << s << " -> " << looksLikeIntGood(s) << '\n';

    std::cout << "same answers -- but the good version built the regex once, "
              << "not " << inputs.size() << " times.\n";
    return 0;
}
