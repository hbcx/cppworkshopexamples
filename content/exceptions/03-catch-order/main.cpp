// When a try has several catch handlers, the FIRST one whose type matches wins
// -- not the closest match. So list the specific (derived) types first and the
// general base last; otherwise the base handler catches everything and the
// specific ones become dead code (the compiler warns about that).
#include <iostream>
#include <stdexcept>

void fail(int which) {
    if (which == 0) throw std::out_of_range("index past end");
    if (which == 1) throw std::invalid_argument("bad argument");
    throw std::runtime_error("something else");
}

int main() {
    for (int i = 0; i < 3; ++i) {
        try {
            fail(i);
        }
        // Specific first...
        catch (const std::out_of_range& e) {
            std::cout << "out_of_range: " << e.what() << '\n';
        }
        catch (const std::invalid_argument& e) {
            std::cout << "invalid_argument: " << e.what() << '\n';
        }
        // ...general base last: the catch-all for the rest of the family.
        catch (const std::exception& e) {
            std::cout << "other: " << e.what() << '\n';
        }
    }
    return 0;
}
