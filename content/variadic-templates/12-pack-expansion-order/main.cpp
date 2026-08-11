#include <iostream>

int step = 0;
int tick(const char* label) {
    ++step;
    std::cout << "  step " << step << ": " << label << "\n";
    return step;
}

// GOOD: a comma fold evaluates its elements strictly LEFT TO RIGHT, guaranteed.
template <class... Ts>
void orderedSideEffects(Ts... args) {
    ((void)tick(args), ...);
}

void takeThree(int, int, int) {}   // stand-in for a function we expand a pack into

int main() {
    std::cout << "ordered (comma fold):\n";
    step = 0;
    orderedSideEffects("a", "b", "c");   // always a, then b, then c

    // BAD (described, not run, because its order is unspecified): expanding a pack
    // into a FUNCTION CALL's arguments --
    //   takeThree(tick("a"), tick("b"), tick("c"));
    // -- may run the tick() calls in ANY order, because function-argument
    // evaluation order is unspecified. Compilers differ; do not rely on it. Use a
    // comma fold or a braced-init-list when the side-effect order matters.
    (void)&takeThree;
    std::cout << "(a function-call expansion could run these in any order)\n";
    return 0;
}
