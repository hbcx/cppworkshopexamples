#include <iostream>
#include <string>

// BAD: this "generic" function is only ever called with std::string, and its body
// only works for string-like types anyway. Templating it buys nothing and costs a
// header-only definition plus worse errors for a wrong type.
template <class T>
T shout(T s) { return s + "!"; }

// GOOD: a plain function. One clear signature, normal errors, and it can live in a
// .cpp. Reach for a template only when several unrelated types need the same logic.
std::string shoutPlain(std::string s) { return s + "!"; }

int main() {
    std::cout << shout(std::string("hi")) << "\n";   // hi!
    std::cout << shoutPlain("hello") << "\n";        // hello!

    // The template buys nothing here: only one type is ever used, and a wrong type
    // like shout(42) would fail deep inside with "no operator+", where the plain
    // function rejects a bad call cleanly at the call site.
    return 0;
}
