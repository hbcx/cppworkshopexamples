#include <functional>
#include <iostream>
#include <set>
#include <string>

int main() {
    std::cout << std::boolalpha;

    // The void specialization std::less<> (and greater<>, equal_to<>, ...) is
    // TRANSPARENT: it deduces its argument types at the call instead of fixing
    // one type, so it compares different types without converting them first.
    std::less<> cmp;
    std::cout << "cmp(2, 3.5) = " << cmp(2, 3.5) << "\n";   // true  (int vs double)
    std::cout << "cmp(10L, 3) = " << cmp(10L, 3) << "\n";   // false (long vs int)

    // The practical payoff is HETEROGENEOUS LOOKUP in an ordered container. A set
    // with a transparent comparator can be searched by any type that merely
    // compares with the key -- here a string literal / const char* -- WITHOUT
    // constructing a temporary std::string key.
    std::set<std::string, std::less<>> names{"alice", "bob", "carol"};

    auto it = names.find("bob");   // compares the literal directly; builds no std::string
    std::cout << "found bob? " << (it != names.end()) << "\n";   // true

    // A plain std::set<std::string> (default std::less<std::string>) would build
    // a temporary std::string("bob") for this same lookup -- see the anti-pattern.
    return 0;
}
