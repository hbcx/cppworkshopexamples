#include <cstdlib>
#include <cstring>
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
    ~Finally() { if (active_) action_(); }
};
template <typename F> Finally<F> finally(F a) { return Finally<F>(std::move(a)); }

int main() {
    // A C resource with no RAII wrapper: allocate with malloc and guarantee the
    // matching free on every exit path with finally. No manual free anywhere.
    char* buf = static_cast<char*>(std::malloc(16));
    if (buf == nullptr)
        return 1;
    auto cleanup = finally([&] { std::free(buf); std::cout << "  freed the buffer\n"; });

    std::strcpy(buf, "hello");
    std::cout << "buffer holds: " << buf << "\n";
    return 0;   // cleanup runs here
}
