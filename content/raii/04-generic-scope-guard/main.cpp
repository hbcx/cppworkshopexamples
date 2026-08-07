// Writing a whole class per resource is a lot of boilerplate when all you need
// is "run this bit of code when the scope exits". A generic scope guard stores
// a callable and runs it in its destructor. One small template covers locks,
// rollbacks, restoring state, and C-API cleanup -- anything you can put in a
// lambda.
//
// It also supports dismiss(): call it on the success path so the cleanup does
// NOT run. That turns the guard into commit-or-rollback -- roll back by default,
// dismissed once the work succeeds.

#include <iostream>
#include <utility>

template <class F>
class ScopeGuard {
public:
    explicit ScopeGuard(F f) : f_(std::move(f)), active_(true) {}
    ~ScopeGuard() { if (active_) f_(); }        // run the cleanup unless dismissed

    void dismiss() { active_ = false; }         // cancel the cleanup

    // Movable but not copyable: the action must run at most once.
    ScopeGuard(ScopeGuard&& o) noexcept : f_(std::move(o.f_)), active_(o.active_) { o.active_ = false; }
    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;
private:
    F f_;
    bool active_;
};

// C++11 has no class template argument deduction, so a factory deduces F.
template <class F>
ScopeGuard<F> guard(F f) { return ScopeGuard<F>(std::move(f)); }

// A multi-step operation: roll back unless it reaches the commit.
void transfer(bool succeed) {
    std::cout << "begin transaction\n";
    auto rollback = guard([] { std::cout << "  ROLLBACK\n"; });

    // ... do the work ...
    if (!succeed) {
        std::cout << "  work failed, leaving early\n";
        return;                     // rollback fires on the way out
    }
    std::cout << "  work ok\n";
    rollback.dismiss();             // success -> cancel the rollback
    std::cout << "  COMMIT\n";
}

int main() {
    // 1. Plain cleanup: always runs at scope exit.
    {
        auto g = guard([] { std::cout << "cleanup ran\n"; });
        std::cout << "working...\n";
    }   // g's destructor -> "cleanup ran"

    std::cout << "---\n";
    transfer(false);   // rolls back
    std::cout << "---\n";
    transfer(true);    // commits, no rollback
}
