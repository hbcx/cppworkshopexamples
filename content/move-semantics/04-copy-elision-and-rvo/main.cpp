#include <iostream>
#include <string>
#include <utility>

struct Widget {
    std::string name;
    explicit Widget(std::string n) : name(std::move(n)) { std::cout << "ctor(" << name << ")\n"; }
    Widget(const Widget& o) : name(o.name) { std::cout << "copy(" << name << ")\n"; }
    Widget(Widget&& o) noexcept : name(std::move(o.name)) { std::cout << "move\n"; }
    Widget& operator=(const Widget& o) { std::cout << "copy-assign\n"; name = o.name; return *this; }
    Widget& operator=(Widget&& o) noexcept { std::cout << "move-assign\n"; name = std::move(o.name); return *this; }
    ~Widget() { std::cout << "dtor(" << name << ")\n"; }
};

// Returns a prvalue -> guaranteed copy elision since C++17. The object is
// constructed directly in the caller's storage; no move/copy exists even in
// principle (this compiles even if the move ctor were deleted).
Widget makePrvalue() { return Widget("prvalue"); }

// Returns a named local -> eligible for NRVO. The standard only permits it, but
// every mainstream compiler elides here: one ctor, no move.
Widget makeNrvo() {
    Widget w("nrvo");
    return w;
}

// ANTI-PATTERN: std::move on a local return value defeats NRVO. It forces a
// move where the compiler could have elided the object completely.
Widget makePessimised() {
    Widget w("pessimised");
    return std::move(w);            // do NOT do this
}

int main() {
    std::cout << "--- prvalue return: guaranteed elision (C++17) ---\n";
    Widget a = makePrvalue();       // exactly one ctor

    std::cout << "--- NRVO: named local return ---\n";
    Widget b = makeNrvo();          // one ctor, compiler elides the move

    std::cout << "--- return std::move(local): pessimisation ---\n";
    Widget c = makePessimised();    // ctor + move + dtor(moved-from source)

    std::cout << "--- assignment is NOT copy elision ---\n";
    Widget d("existing");
    d = Widget("assigned");         // temp ctor + move-assign + temp dtor

    std::cout << "--- end of scope ---\n";
    return 0;
}
