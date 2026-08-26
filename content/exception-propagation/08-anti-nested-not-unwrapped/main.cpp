// Anti-pattern: wrapping with std::throw_with_nested to add context, then only
// printing the OUTER exception's what(). The nested cause -- the real, original
// error -- is still attached, but a handler that ignores it throws away exactly
// the information the nesting was meant to preserve. Always unwrap the chain.
#include <iostream>
#include <exception>
#include <stdexcept>

void low() { throw std::runtime_error("connection refused"); }

void high() {
    try {
        low();
    }
    catch (...) {
        std::throw_with_nested(std::runtime_error("request failed"));
    }
}

// WRONG: print only the outer message. The cause is lost to the reader.
void report_wrong(const std::exception& e) {
    std::cout << "wrong: " << e.what() << '\n';
}

// RIGHT: walk the nested chain so the real cause is reported too.
void report_right(const std::exception& e, int depth = 0) {
    std::cout << (depth == 0 ? "right: " : "  caused by: ") << e.what() << '\n';
    try {
        std::rethrow_if_nested(e);
    }
    catch (const std::exception& cause) {
        report_right(cause, depth + 1);
    }
}

int main() {
    try {
        high();
    }
    catch (const std::exception& e) {
        report_wrong(e);      // outer only -- "connection refused" is missing
        report_right(e);      // full chain
    }
    return 0;
}
