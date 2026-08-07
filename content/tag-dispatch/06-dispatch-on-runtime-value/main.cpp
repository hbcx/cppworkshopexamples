#include <iostream>

// Tag types and their overloads.
struct use_fast {};
struct use_safe {};
int compute(int x, use_fast) { return x << 1; }
int compute(int x, use_safe) { return x + x; }

// COMPILE-TIME choice: the tag is a template argument, fixed when run is
// instantiated. This is what tag dispatch is for.
template <class Tag>
int run(int x) { return compute(x, Tag{}); }

// RUN-TIME choice: the flag is only known while the program runs, so it cannot
// select an overload by type. A plain if is the right tool -- and it works here
// because BOTH paths are valid for x.
int runtimeChoice(int x, bool fast) {
    if (fast) return compute(x, use_fast{});
    else      return compute(x, use_safe{});
}

int main() {
    std::cout << "compile-time (tag dispatch): " << run<use_fast>(21) << "\n";

    bool flag = (2 + 2 == 4);      // stand-in for a value read at run time
    std::cout << "run-time (plain if):        " << runtimeChoice(21, flag) << "\n";

    // THE ANTI-PATTERN (does not compile, shown as a comment):
    //   bool fast = flag;
    //   return compute(x, fast ? use_fast{} : use_safe{});
    // The conditional has two DIFFERENT tag types and no common type, so the
    // expression is ill-formed. A tag selects an overload by TYPE, known at
    // compile time; a value known only at run time cannot pick a type.
    return 0;
}
