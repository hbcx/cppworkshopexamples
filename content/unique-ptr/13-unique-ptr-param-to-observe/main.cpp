#include <iostream>
#include <memory>
#include <utility>

struct Widget {
    int value;
    explicit Widget(int v) : value(v) {}
};

// Anti-pattern A: takes the unique_ptr BY VALUE just to read it. This is a sink
// signature, so every caller is forced to surrender ownership even though the
// function only looks at the object:
//     static int value_bad(std::unique_ptr<Widget> w) { return w->value; }
//
// Anti-pattern B: takes unique_ptr<Widget>& -- now the function is welded to
// this ONE ownership model and cannot be called on a stack Widget, a
// shared_ptr, or a raw pointer from a C API:
//     static int value_ref(const std::unique_ptr<Widget>& w) { return w->value; }

// The fix: to OBSERVE, take a plain reference (or a raw pointer if it may be
// null). The function no longer cares how the Widget is owned.
static int value_of(const Widget& w) { return w.value; }

// Take the unique_ptr by value ONLY when the function is a genuine sink that
// keeps ownership.
static std::unique_ptr<Widget> owned;
static void take_ownership(std::unique_ptr<Widget> w) { owned = std::move(w); }

int main() {
    std::unique_ptr<Widget> p(new Widget(7));

    // Observing does not touch ownership: p still owns the Widget afterwards.
    std::cout << "value = " << value_of(*p) << '\n';
    std::cout << "p still owns: " << (p ? "yes" : "no") << '\n';

    // The same observer works on a Widget that no unique_ptr owns at all:
    Widget local(11);
    std::cout << "value = " << value_of(local) << '\n';

    // Transfer only when you really mean to hand ownership over:
    take_ownership(std::move(p));
    std::cout << "after handing over, p owns: " << (p ? "yes" : "no") << '\n';
    std::cout << "stored value = " << value_of(*owned) << '\n';
    return 0;
}
