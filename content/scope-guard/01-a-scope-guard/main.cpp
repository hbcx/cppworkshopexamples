#include <iostream>
#include <utility>

// A minimal scope guard: it stores a callable and runs it in its destructor, so
// the cleanup happens when the guard leaves scope -- normal exit, early return, or
// an exception. Class template argument deduction (C++17) lets you write
// ScopeGuard g{lambda} without naming the lambda's type.
template <typename F>
class ScopeGuard {
    F action_;
public:
    explicit ScopeGuard(F action) : action_(std::move(action)) {}
    ~ScopeGuard() { action_(); }
    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;
};

void work(bool earlyReturn) {
    ScopeGuard g{[] { std::cout << "  cleanup ran\n"; }};
    std::cout << "  working...\n";
    if (earlyReturn) {
        std::cout << "  early return\n";
        return;                 // cleanup still runs
    }
    std::cout << "  finished normally\n";
}

int main() {
    std::cout << "normal path:\n";
    work(false);
    std::cout << "early-return path:\n";
    work(true);
    return 0;
}
