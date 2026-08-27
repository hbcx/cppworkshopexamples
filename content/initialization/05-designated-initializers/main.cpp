#include <iostream>
#include <string>

// Designated initializers (C++20) name the member being initialized, so an
// aggregate with several fields reads clearly and you can skip the ones you want
// value-initialized. The ".host = ..." looks like assignment, but it is
// INITIALIZATION: each member is built with that value as the object is created,
// never assigned afterwards. C++ is stricter than C here -- the designators must
// be in DECLARATION order, you cannot mix designated and plain initializers, and
// it works only for aggregates.

struct Config {
    std::string host;
    int         port;
    int         timeout;
    bool        secure;
};

struct Rect {
    int width;
    int height;
};

int main() {
    // Name each member. This reads far better than a bare {..., ..., ...} when
    // the struct has several fields, especially several of the same type.
    Config c{.host = "localhost", .port = 8080, .timeout = 30, .secure = true};
    std::cout << c.host << ":" << c.port
              << " timeout=" << c.timeout
              << " secure=" << (c.secure ? "yes" : "no") << "\n";

    // Skipped members are value-initialized: timeout becomes 0 and secure false.
    // You may skip members, but you may not reorder them. Both compilers warn
    // that you left fields out (-Wmissing-field-initializers) -- a useful nudge;
    // this example turns -Werror off to show the behaviour.
    Config partial{.host = "db", .port = 5432};
    std::cout << partial.host << ":" << partial.port
              << " timeout=" << partial.timeout
              << " secure=" << (partial.secure ? "yes" : "no") << "\n";  // 0, no

    // It is still aggregate initialization, so narrowing is caught here too:
    //     Rect bad{.width = 3.5};   // error: narrowing double -> int
    Rect r{.width = 640, .height = 480};
    std::cout << "Rect " << r.width << "x" << r.height << "\n";

    // Rejected forms (shown as comments -- they do not compile):
    //     Config x{.port = 80, .host = "h"};   // error: designators out of order
    //     Rect y{.width = 1, 480};             // error: mixes designated + plain
    return 0;
}
