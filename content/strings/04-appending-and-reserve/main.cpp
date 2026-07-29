#include <iostream>
#include <string>

// Build a 60-char string one piece at a time and report whether the buffer ever
// moved (a reallocation). We compare data() pointers rather than counting exact
// reallocations, whose number depends on the library's growth policy.
static bool bufferMovedWhileBuilding(bool withReserve) {
    std::string s;
    if (withReserve) s.reserve(64);        // capacity up front; size stays 0
    const char* first = nullptr;
    bool moved = false;
    for (int i = 0; i < 60; ++i) {
        s.push_back('a');
        const char* now = s.data();
        if (first == nullptr) first = now;
        else if (now != first) { moved = true; first = now; }
    }
    return moved;
}

int main() {
    // In place: += and append grow one buffer, amortized linear.
    std::string url;
    url += "https://";
    url.append("example.com");
    url += '/';
    url.append(3, 'x');                    // count + fill
    std::cout << "built: " << url << " (size " << url.size() << ")\n";

    std::cout << "without reserve, buffer moved? "
              << (bufferMovedWhileBuilding(false) ? "yes" : "no") << '\n';
    std::cout << "with reserve,    buffer moved? "
              << (bufferMovedWhileBuilding(true) ? "yes" : "no") << '\n';

    // reserve changes capacity, not size: still empty, ready to be filled.
    std::string r;
    r.reserve(100);
    std::cout << "after reserve(100): size " << r.size()
              << ", capacity >= 100? " << (r.capacity() >= 100 ? "yes" : "no") << '\n';
    return 0;
}
