#include <iostream>
#include <stdexcept>
#include <utility>

template <typename F>
class Finally {
    F action_;
public:
    explicit Finally(F a) : action_(std::move(a)) {}
    Finally(const Finally&) = delete;
    // A destructor is noexcept by default. If action_ throws here -- especially
    // while the stack is already unwinding from another exception -- the program
    // calls std::terminate. A cleanup must not throw.
    ~Finally() noexcept { action_(); }
};
template <typename F> Finally<F> finally(F a) { return Finally<F>(std::move(a)); }

// ANTI-PATTERN (described): a cleanup that can throw.
//   auto g = finally([&]{ risky_close(); });   // if risky_close throws -> terminate
//
// FIX: contain any failure inside the cleanup -- log it, swallow it, set a flag --
// but never let it escape the destructor.
void safeClose() {
    try {
        throw std::runtime_error("close failed");   // stand-in for a failing release
    } catch (const std::exception& e) {
        std::cout << "  cleanup: swallowed error (" << e.what() << ")\n";
    }
}

int main() {
    auto g = finally([] { safeClose(); });   // never throws out of the destructor
    std::cout << "working\n";
    return 0;
}
