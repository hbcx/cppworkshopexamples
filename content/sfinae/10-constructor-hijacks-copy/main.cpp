#include <iostream>
#include <string>
#include <type_traits>

// A "universal" constructor -- template <class U> C(U&&) -- is greedy: for a
// non-const lvalue C it is a BETTER match than the copy constructor C(const C&),
// so copying such a C calls the template instead of copying. enable_if is the
// classic fix: exclude the class's own type from the template constructor.

struct Bad {
    std::string label;
    // Meant to accept any argument that builds the label...
    template <class U>
    Bad(U&&) : label("from template") { std::cout << "  template ctor\n"; }
    Bad(const Bad&) : label("from copy") { std::cout << "  copy ctor\n"; }
};

struct Good {
    std::string label;
    // ...but exclude Good itself. decay strips reference and const, so the check
    // catches Good&, const Good&, Good&&, etc. -- every way of naming the own type.
    template <class U,
              typename std::enable_if<
                  !std::is_same<typename std::decay<U>::type, Good>::value, int>::type = 0>
    Good(U&&) : label("from template") { std::cout << "  template ctor\n"; }
    Good(const Good&) : label("from copy") { std::cout << "  copy ctor\n"; }
};

int main() {
    std::cout << "Bad:\n";
    Bad b1("hello");         // template ctor -- correct, from a string literal
    Bad b2 = b1;             // wanted a copy, but the template is the better match
    std::cout << "  b2.label = " << b2.label << "\n";   // "from template" -- the bug

    std::cout << "Good:\n";
    Good g1("hello");        // template ctor -- correct
    Good g2 = g1;            // template excluded for Good -> real copy ctor runs
    std::cout << "  g2.label = " << g2.label << "\n";   // "from copy"
    return 0;
}
