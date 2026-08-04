// Anti-pattern: a chain where a lookup table is the right tool.
//
// Chain of Responsibility earns its indirection when handlers have ranges,
// priorities, or overlapping conditions, or may decline dynamically. When every
// handler just checks the request against one exact key, the chain degenerates
// into a linear scan of equality tests -- O(n) per request, the selection logic
// scattered across handlers -- and what you actually have is keyed dispatch,
// which a hash map does in O(1) and in one readable place.

#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include <unordered_map>

int main() {
    // ---- BAD: a "chain" of exact-key handlers = a linear scan of if (cmd == X) ----
    using Handler = std::function<bool(const std::string&)>;
    std::vector<Handler> chain = {
        [](const std::string& c) { if (c == "start")  { std::cout << "  starting\n";  return true; } return false; },
        [](const std::string& c) { if (c == "stop")   { std::cout << "  stopping\n";  return true; } return false; },
        [](const std::string& c) { if (c == "status") { std::cout << "  status ok\n"; return true; } return false; },
    };
    auto dispatchChain = [&chain](const std::string& cmd) {
        for (const Handler& h : chain)
            if (h(cmd)) return;
        std::cout << "  unknown: " << cmd << "\n";
    };

    // ---- GOOD: a hash map -- direct O(1) dispatch, all routes in one place ----
    std::unordered_map<std::string, std::function<void()>> table = {
        { "start",  [] { std::cout << "  starting\n"; } },
        { "stop",   [] { std::cout << "  stopping\n"; } },
        { "status", [] { std::cout << "  status ok\n"; } },
    };
    auto dispatchMap = [&table](const std::string& cmd) {
        auto it = table.find(cmd);
        if (it != table.end()) it->second();
        else std::cout << "  unknown: " << cmd << "\n";
    };

    std::cout << "chain:\n";
    dispatchChain("stop");
    dispatchChain("nope");
    std::cout << "map:\n";
    dispatchMap("stop");
    dispatchMap("nope");
    return 0;
}
