#include <iostream>
#include <string>

// Count how many times the buffer's address changes while appending 200 pieces
// two different ways. A reallocation moves the characters; more moves = more work.
static int reallocsWithPlus() {
    std::string s;
    const char* last = s.data();
    int moves = 0;
    for (int i = 0; i < 200; ++i) {
        s = s + "ab";                      // builds a whole new string each time
        if (s.data() != last) { ++moves; last = s.data(); }
    }
    return moves;
}

static int reallocsWithAppend() {
    std::string s;
    const char* last = s.data();
    int moves = 0;
    for (int i = 0; i < 200; ++i) {
        s += "ab";                         // grows one buffer in place
        if (s.data() != last) { ++moves; last = s.data(); }
    }
    return moves;
}

int main() {
    int plus = reallocsWithPlus();
    int append = reallocsWithAppend();
    std::cout << "s = s + piece : buffer moved " << plus << " times (out of 200)\n";
    std::cout << "s += piece    : buffer moved " << append << " times (out of 200)\n";
    std::cout << "+= reallocates far less often? " << (append < plus ? "yes" : "no") << '\n';
    return 0;
}
