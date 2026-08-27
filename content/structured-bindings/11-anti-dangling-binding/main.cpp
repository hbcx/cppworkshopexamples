#include <iostream>
#include <string>
#include <utility>

// Anti-pattern: a reference binding that outlives what it refers to. auto&& [..]
// binds to the hidden object and extends a temporary's lifetime for as long as
// the binding lives -- but only that long. Returning a reference to a binding
// name, or keeping a pointer to it past the enclosing scope, dangles just like
// any reference to a local.

std::pair<std::string, std::string> makeNames() {
    return {"first", "second"};
}

// A reference binding to a temporary IS valid while the binding is in scope:
void useHere() {
    auto&& [a, b] = makeNames();   // the temporary's lifetime is extended to a/b
    std::cout << "in scope: " << a << ", " << b << "\n";
}   // the hidden temporary is destroyed here

// DANGLING (shown as a comment -- never do this):
//     const std::string& firstName() {
//         auto [a, b] = makeNames();   // a, b are local to this function
//         return a;                    // returns a reference to a destroyed local
//     }

int main() {
    useHere();

    // If a name must outlive the current scope, bind by VALUE (a copy) and keep
    // that copy -- do not return or store a reference into a binding.
    auto [a, b] = makeNames();     // a, b are copies here
    std::string kept = a;          // an independent copy, safe to keep
    std::cout << "kept safely: " << kept << " (" << b << ")\n";
    return 0;
}
