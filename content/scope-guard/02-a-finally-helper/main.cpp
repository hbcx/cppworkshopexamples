#include <iostream>
#include <utility>

// A reusable finally: a move-only guard plus a factory function so callers write
// auto _ = finally([&]{ ... }); without spelling the type. The move constructor
// transfers the responsibility and marks the source inactive, so the action runs
// exactly once.
template <typename F>
class Finally {
    F action_;
    bool active_ = true;
public:
    explicit Finally(F action) : action_(std::move(action)) {}
    Finally(Finally&& o) noexcept : action_(std::move(o.action_)), active_(o.active_) {
        o.active_ = false;
    }
    Finally(const Finally&) = delete;
    ~Finally() { if (active_) action_(); }
};

template <typename F>
Finally<F> finally(F action) {
    return Finally<F>(std::move(action));
}

int main() {
    {
        auto guard = finally([] { std::cout << "  released\n"; });
        std::cout << "  using the resource\n";
    }   // guard destroyed here
    std::cout << "after the block\n";
    return 0;
}
