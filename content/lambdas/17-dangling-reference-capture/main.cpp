#include <iostream>
#include <functional>

// GOOD: capture by value, so the returned closure owns a copy that lives exactly
// as long as the closure does.
static std::function<int()> makeCounter() {
    int value = 42;
    return [value] { return value; };   // by value -- safe to outlive makeCounter
}

int main() {
    // BAD (shown in a comment -- it is a run-time dangling bug, undefined behavior):
    //
    //   std::function<int()> makeBad() {
    //       int local = 42;
    //       return [&local] { return local; };   // reference to a local that dies
    //   }
    //   auto bad = makeBad();
    //   bad();   // reads a destroyed variable: UB -- may print 42, garbage, or crash
    //
    // A reference capture is valid only as long as what it points at. When the
    // closure ESCAPES its scope -- returned, stored, run later -- a [&] capture of
    // a local dangles. Capture BY VALUE whatever the closure must keep.

    auto counter = makeCounter();
    std::cout << "value captured by value: " << counter() << '\n';   // 42

    // Reference capture is the right choice when the closure runs WITHIN the
    // referent's lifetime -- an inline predicate to std::sort that runs at once.
    return 0;
}
