#include <iomanip>
#include <iostream>

int main() {
    // The key mental model: almost every manipulator is STICKY -- it stays in
    // effect until you change it. setw is the ONE exception: it applies to the
    // very next output and then resets to 0.
    std::cout << std::setfill('*');           // sticky
    std::cout << std::setw(6) << 1 << '\n';   // *****1
    std::cout << std::setw(6) << 2 << '\n';   // *****2  (setfill still '*', setw re-applied)
    std::cout << 3 << '\n';                   // 3       (no setw, so no padding)

    // setfill kept padding; setw did not carry over -- you must set it per field.
    std::cout << std::setfill(' ');           // reset fill so it does not leak
    return 0;
}
