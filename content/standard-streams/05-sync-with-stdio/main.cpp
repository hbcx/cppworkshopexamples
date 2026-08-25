#include <iostream>

int main() {
    // By default the C++ streams are kept in sync with C stdio (printf/scanf),
    // so mixing cout and printf produces output in the right order. That sync
    // has a cost. sync_with_stdio(false) turns it off for a significant speedup
    // on bulk I/O -- but then C and C++ output are buffered independently and may
    // interleave out of order, so do not mix them afterwards.
    //
    // Call it ONCE, before any I/O, at the top of main. It returns the previous
    // setting.
    bool wasSynced = std::ios_base::sync_with_stdio(false);
    std::cout << "streams were synced before: " << wasSynced << '\n';   // 1

    // With sync off, use ONLY C++ streams (no printf) from here on.
    std::cout << "now using C++ streams only, unsynced\n";

    // The usual fast-I/O prelude at the top of a program:
    //   std::ios_base::sync_with_stdio(false);
    //   std::cin.tie(nullptr);
    return 0;
}
