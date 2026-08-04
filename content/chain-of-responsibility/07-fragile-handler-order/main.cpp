// Anti-pattern: a chain whose correctness depends on fragile ordering.
//
// In a first-match chain, a general handler placed before a specific one
// swallows the requests the specific handler was meant to take: the general
// handler accepts everything, so the traversal never reaches the specific one.
// The chain still "works" -- every request is handled -- which is what makes the
// bug easy to miss; it is just handled by the WRONG handler. Order handlers from
// most specific to most general, with any catch-all last.

#include <iostream>
#include <functional>
#include <vector>
#include <string>

int main() {
    using Handler = std::function<bool(const std::string&)>; // true == handled

    auto adminHandler = [](const std::string& path) {
        if (path == "/admin") { std::cout << "  " << path << " -> admin handler\n"; return true; }
        return false;
    };
    auto catchAll = [](const std::string& path) {
        std::cout << "  " << path << " -> generic handler\n";
        return true; // accepts everything
    };

    // BAD: the catch-all is first, so it swallows /admin before the admin
    // handler is ever consulted.
    std::vector<Handler> bad = { catchAll, adminHandler };
    // GOOD: specific first, general last as a fallback.
    std::vector<Handler> good = { adminHandler, catchAll };

    auto dispatch = [](const std::vector<Handler>& chain, const std::string& path) {
        for (const Handler& h : chain)
            if (h(path)) return;
    };

    std::cout << "bad order, /admin:\n";  dispatch(bad, "/admin");  // wrongly generic
    std::cout << "good order, /admin:\n"; dispatch(good, "/admin"); // correctly admin
    std::cout << "good order, /home:\n";  dispatch(good, "/home");  // falls through to generic
    return 0;
}
