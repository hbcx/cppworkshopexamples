// Anti-pattern: a catch(...) that silently swallows every exception. It makes
// code look robust while actually HIDING failures -- the operation reports
// success, downstream code trusts a result that was never produced, and the
// real error disappears with no message. Catch specific types you can handle,
// and at least log-and-rethrow the rest.
#include <iostream>
#include <stdexcept>
#include <string>

// A parse step that fails on bad input.
int parse(const std::string& text) {
    if (text.empty()) throw std::runtime_error("empty input");
    return static_cast<int>(text.size());
}

// WRONG: swallow everything and return a default. The caller cannot tell a real
// value from a masked failure.
int load_wrong(const std::string& text) {
    try {
        return parse(text);
    }
    catch (...) {                           // silent -- the error vanishes
        return 0;
    }
}

// RIGHT: log the failure and rethrow, so the caller still gets to decide.
int load_right(const std::string& text) {
    try {
        return parse(text);
    }
    catch (const std::exception& e) {
        std::cout << "log: parse failed: " << e.what() << '\n';
        throw;                              // let it propagate
    }
}

int main() {
    // The swallowed version looks like it succeeded with 0 -- indistinguishable
    // from a real zero-length result.
    std::cout << "wrong: " << load_wrong("") << '\n';

    // The correct version surfaces the failure instead of masking it.
    try {
        load_right("");
    }
    catch (const std::exception& e) {
        std::cout << "right: propagated: " << e.what() << '\n';
    }
    return 0;
}
