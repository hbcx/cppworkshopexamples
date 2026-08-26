// A trace reflects the real call depth: a snapshot taken deeper in the call chain
// has more frames than one taken higher up. This is why a stacktrace pinpoints
// the path taken -- each nested call adds a frame. We compare sizes rather than
// print them, since the absolute count depends on the runtime's startup frames.
#include <stacktrace>
#include <iostream>
#include <cstddef>

std::size_t at_depth_2() {
    return std::stacktrace::current().size();
}
std::size_t at_depth_1() {
    return at_depth_2();
}

int main() {
    std::size_t shallow = std::stacktrace::current().size();   // captured in main
    std::size_t deep = at_depth_1();                           // two calls deeper

    std::cout << std::boolalpha;
    std::cout << "deeper stack has more frames: " << (deep > shallow) << '\n';   // true
    return 0;
}
