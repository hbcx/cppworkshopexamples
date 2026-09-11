#include <exception>
#include <iostream>
#include <stdexcept>
#include <utility>

// scope_success runs its action only on a NORMAL exit; scope_fail only when the
// scope is left because of an exception. Each records the count of in-flight
// exceptions at construction and compares it at destruction with
// std::uncaught_exceptions() -- if the count went up, we are unwinding.
template <typename F, bool OnFail>
class ScopeCond {
    F action_;
    int entryExceptions_ = std::uncaught_exceptions();
public:
    explicit ScopeCond(F a) : action_(std::move(a)) {}
    ScopeCond(const ScopeCond&) = delete;
    ~ScopeCond() {
        bool leavingByException = std::uncaught_exceptions() > entryExceptions_;
        if (leavingByException == OnFail)
            action_();
    }
};

template <typename F> ScopeCond<F, true>  scope_fail(F a)    { return ScopeCond<F, true>(std::move(a)); }
template <typename F> ScopeCond<F, false> scope_success(F a) { return ScopeCond<F, false>(std::move(a)); }

void doWork(bool fail) {
    auto onOk   = scope_success([] { std::cout << "  success action\n"; });
    auto onFail = scope_fail   ([] { std::cout << "  failure action\n"; });
    std::cout << "  doing work (fail=" << std::boolalpha << fail << ")\n";
    if (fail)
        throw std::runtime_error("boom");
}

int main() {
    std::cout << "normal:\n";
    doWork(false);
    std::cout << "throwing:\n";
    try {
        doWork(true);
    } catch (const std::exception& e) {
        std::cout << "  caught: " << e.what() << "\n";
    }
    return 0;
}
