#include <iostream>
#include <utility>

template <typename F>
class Finally {
    F action_;
public:
    explicit Finally(F a) : action_(std::move(a)) {}
    Finally(const Finally&) = delete;
    ~Finally() { action_(); }
};
template <typename F> Finally<F> finally(F a) { return Finally<F>(std::move(a)); }

int main() {
    // ANTI-PATTERN: not naming the guard. A temporary is destroyed at the end of
    // the full expression -- immediately -- so the cleanup runs NOW, before the
    // work it was meant to guard.
    finally([] { std::cout << "  cleanup (fired immediately!)\n"; });
    std::cout << "  work that should have been guarded\n";

    std::cout << "---\n";

    // FIX: bind the guard to a named variable so it lives until the scope ends.
    auto guard = finally([] { std::cout << "  cleanup (at scope exit)\n"; });
    std::cout << "  guarded work\n";
    return 0;
}
