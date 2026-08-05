// Anti-pattern: deriving from Base<SomeOtherType> instead of Base<Self>.
//
// CRTP's static_cast trusts that `this` really is Derived. If a class passes the
// WRONG type -- struct Bad : Registry<Good> -- the base casts a Bad to Good and
// calls Good's method on it: undefined behaviour. We describe it; we do NOT run
// the broken cast.
//
// The fix: pass the class's own type, and let a static_assert catch the slip.

#include <iostream>
#include <string>
#include <type_traits>

// A CRTP base that reads the derived type's name via a static_cast.
template <class Derived>
class Registry {
public:
    std::string describe() const {
        // Guard: turns the wrong-parameter slip into a COMPILE error instead of
        // runtime UB. Bad : Registry<Good> would fail this assert.
        static_assert(std::is_base_of<Registry<Derived>, Derived>::value,
                      "Derived must pass itself as the CRTP argument");
        return static_cast<const Derived&>(*this).name();
    }
};

// Correct: each class passes ITSELF.
struct Good : Registry<Good> {
    std::string name() const { return "Good"; }
};

struct Also : Registry<Also> {
    std::string name() const { return "Also"; }
};

// WRONG (would be UB, and here the static_assert would reject it):
//     struct Bad : Registry<Good> {          // copy-paste slip: not Registry<Bad>
//         std::string name() const { return "Bad"; }
//     };
//     Bad b; b.describe();  // static_cast<Good&>(*this) on a Bad -> UB
// Left un-instantiated so nothing undefined runs.

int main() {
    Good g;
    Also a;
    std::cout << "describe: " << g.describe() << "\n";
    std::cout << "describe: " << a.describe() << "\n";
    std::cout << "(the Bad : Registry<Good> slip is caught by static_assert)\n";
}
