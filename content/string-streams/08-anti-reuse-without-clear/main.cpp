#include <iostream>
#include <sstream>
#include <string>

int main() {
    // Anti-pattern: reusing a stringstream by calling str() but NOT clear().
    // After the first read hits the end, eofbit/failbit are set; str() swaps the
    // content but leaves those bits, so every read on the "new" content fails.
    std::istringstream in("111 222");
    int x = 0;
    while (in >> x) { }          // reads to the end -> failbit is now set

    in.str("333 444");           // new content, but state NOT reset
    int y = -1;
    in >> y;                     // fails immediately: failbit still set
    std::cout << "without clear, read ok: " << static_cast<bool>(in) << '\n'; // 0
    std::cout << "y unchanged: " << (y == -1) << '\n';                        // 1

    // Fix: clear() the state bits before loading (or after swapping) new content.
    in.clear();
    in.str("333 444");
    int z = 0;
    in >> z;
    std::cout << "after clear, read: " << z << '\n';   // 333
    return 0;
}
