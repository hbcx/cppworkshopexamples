#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <string_view>

int main() {
    std::string owned = "clang-format";                      // one std::string, NUL at index 12
    std::string_view tool = std::string_view(owned).substr(0, 5);   // "clang"

    std::cout << "view = " << tool << " (size " << tool.size() << ")\n";

    // WRONG: data() points at 'c', but there is NO '\0' after "clang" -- the next
    // byte is '-'. A C API that stops at '\0' keeps reading PAST the view. This is
    // only defined HERE because the bytes belong to `owned` (itself NUL-terminated);
    // over a buffer that is not null-terminated it would be undefined behaviour.
    std::printf("printf(\"%%s\") on data(): %s\n", tool.data());        // prints clang-format
    std::cout << "strlen(data()) = " << std::strlen(tool.data())        // 12, not 5!
              << " but view size = " << tool.size() << '\n';

    // RIGHT: hand the C API a real, NUL-terminated copy of exactly the view.
    std::string copy(tool);
    std::printf("printf on copy.c_str(): %s\n", copy.c_str());          // prints clang
    std::cout << "strlen(c_str()) = " << std::strlen(copy.c_str()) << '\n';  // 5
    return 0;
}
