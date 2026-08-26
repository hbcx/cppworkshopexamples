// Anti-pattern: misunderstanding const& lifetime extension. Binding a temporary
// to a LOCAL const reference extends the temporary's life to that reference's
// scope -- but only for that direct binding. It does NOT extend across a function
// return, so a function returning a reference to a temporary still dangles.
#include <iostream>
#include <string>

std::string make() { return "computed value"; }

// WRONG (described, not run -- it dangles):
//
//   const std::string& via_return() {
//       return make();     // the temporary is NOT extended past the return,
//   }                      // so the returned reference dangles at the call site.
//   const std::string& x = via_return();   // reading x is undefined behaviour
//
// RIGHT: a local const reference DOES extend the temporary to its own scope.
int main() {
    const std::string& r = make();     // extended: r keeps the temporary alive
    std::cout << "local const& extends the temporary: \"" << r << "\"\n";

    // The extension is a local rule; it never reaches through via_return() above.
    // When in doubt, bind to a value -- copy elision makes it free.
    std::string safe = make();
    std::cout << "binding to a value is always safe:  \"" << safe << "\"\n";
    return 0;
}
