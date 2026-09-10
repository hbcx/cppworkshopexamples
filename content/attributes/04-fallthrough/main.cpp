#include <iostream>

// Each grade earns its own point PLUS everything the lower grades earn, so the
// cases deliberately fall through. Because each case does work before falling
// through, the compiler warns -Wimplicit-fallthrough (an error under -Werror)
// unless you mark the fallthrough as intentional with [[fallthrough]].
int points(char grade) {
    int p = 0;
    switch (grade) {
        case 'A':
            p += 1;
            [[fallthrough]];
        case 'B':
            p += 1;
            [[fallthrough]];
        case 'C':
            p += 1;
            break;
        default:
            p = 0;
    }
    return p;
}

int main() {
    for (char g : {'A', 'B', 'C', 'X'})
        std::cout << g << " -> " << points(g) << "\n";
    return 0;
}
