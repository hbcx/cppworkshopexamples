// Anti-pattern: capturing a stacktrace on a hot path. std::stacktrace::current()
// is expensive -- it walks the stack and resolves symbols/debug info, far more
// costly than the work in a typical loop iteration. Capturing one on every call
// or element turns a fast loop into a slow one. Capture a trace only when you
// actually need it: on the error path, not the happy path.
#include <stacktrace>
#include <iostream>
#include <vector>

// WRONG: a trace captured for every element, even though almost all succeed.
void process_wrong(const std::vector<int>& items) {
    for (int x : items) {
        std::stacktrace here = std::stacktrace::current();   // every iteration!
        (void)here;
        (void)x;
    }
}

// RIGHT: do the work; capture a trace only in the rare failure branch.
bool process_right(const std::vector<int>& items) {
    std::stacktrace failure;                 // empty unless something fails
    for (int x : items) {
        if (x < 0) {                         // the rare error path
            failure = std::stacktrace::current();
            break;
        }
    }
    return !failure.empty();
}

int main() {
    std::cout << std::boolalpha;
    std::vector<int> data = {1, 2, 3, -4, 5};

    process_wrong(data);                      // works, but pays the cost 5 times
    bool captured = process_right(data);      // pays it once, only on the -4

    std::cout << "right captured on the error only: " << captured << '\n';   // true
    return 0;
}
