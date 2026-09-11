#include <iostream>
#include <utility>

template <typename F>
class Finally {
    F action_;
    bool active_ = true;
public:
    explicit Finally(F a) : action_(std::move(a)) {}
    Finally(Finally&& o) noexcept : action_(std::move(o.action_)), active_(o.active_) { o.active_ = false; }
    Finally(const Finally&) = delete;
    ~Finally() noexcept { if (active_) action_(); }   // noexcept: cleanup must not throw
};
template <typename F> Finally<F> finally(F a) { return Finally<F>(std::move(a)); }

// A factory that acquires a resource AND hands back the guard that releases it,
// transferring the cleanup responsibility to the caller. The guard is move-only,
// so the release still happens exactly once -- at the caller's scope exit.
auto acquire(int id) {
    std::cout << "  acquired resource " << id << "\n";
    return finally([id] { std::cout << "  released resource " << id << "\n"; });
}

int main() {
    std::cout << "before acquire\n";
    {
        auto guard = acquire(7);   // guard moved out of acquire(); one release
        std::cout << "  using resource 7\n";
    }   // released here, once
    std::cout << "after block\n";
    return 0;
}
