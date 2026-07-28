#include <iostream>
#include <memory>
#include <utility>

struct Widget {
    int value;
    explicit Widget(int v) : value(v) {}
};

// Anti-pattern: taking shared_ptr BY VALUE just to read the object. Every call
// copies the shared_ptr -- an atomic increment now and an atomic decrement on
// return -- synchronisation the function does not need, and on a hot path it can
// dominate:
//     int read_bad(std::shared_ptr<Widget> w) { return w->value; }

// The fix: to OBSERVE, take a plain reference (or a raw pointer if it may be
// null). No reference count is touched, and it works however the Widget is
// owned -- unique_ptr, shared_ptr, or a stack object.
static int read(const Widget& w) { return w.value; }

// Take shared_ptr BY VALUE only when the function actually keeps a copy, i.e.
// shares ownership (stores it, captures it in a task).
static std::shared_ptr<Widget> g_stored;
static void keep(std::shared_ptr<Widget> w) { g_stored = std::move(w); }

int main() {
    auto w = std::make_shared<Widget>(7);

    std::cout << "value = " << read(*w) << '\n';                       // no count touched
    std::cout << "use_count after read = " << w.use_count() << '\n';   // still 1

    keep(w);   // genuinely sharing ownership: by-value is the right choice here
    std::cout << "use_count after keep = " << w.use_count() << '\n';   // 2
    std::cout << "stored value = " << read(*g_stored) << '\n';
    return 0;
}
