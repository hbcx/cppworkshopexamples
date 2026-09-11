#include <iostream>
#include <utility>
#include <vector>

// A guard you can dismiss: the rollback runs unless the operation reaches the end
// and calls dismiss(). This is the transaction idiom -- do the work under a
// rollback, then commit by dismissing it.
template <typename F>
class ScopeGuard {
    F action_;
    bool active_ = true;
public:
    explicit ScopeGuard(F a) : action_(std::move(a)) {}
    ScopeGuard(ScopeGuard&& o) noexcept : action_(std::move(o.action_)), active_(o.active_) { o.active_ = false; }
    ScopeGuard(const ScopeGuard&) = delete;
    ~ScopeGuard() { if (active_) action_(); }
    void dismiss() { active_ = false; }
};

bool transfer(std::vector<int>& log, bool shouldFail) {
    log.push_back(1);                                       // step 1 committed
    ScopeGuard rollback{[&] { log.pop_back(); std::cout << "  rolled back step 1\n"; }};

    if (shouldFail) {
        std::cout << "  step 2 failed -> rollback fires\n";
        return false;                                       // rollback runs
    }

    log.push_back(2);                                       // step 2 committed
    rollback.dismiss();                                     // success: keep changes
    std::cout << "  committed\n";
    return true;
}

int main() {
    std::vector<int> log;
    transfer(log, true);
    std::cout << "after failed transfer, log size = " << log.size() << "\n";
    transfer(log, false);
    std::cout << "after ok transfer, log size = " << log.size() << "\n";
    return 0;
}
