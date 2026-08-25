#include <print>

int main() {
    // print and println can target a FILE*: stdout by default, or stderr for
    // diagnostics -- keeping errors off the data stream just like std::cerr.
    std::println(stdout, "result: {}", 42);                 // to stdout (the data)
    std::println(stderr, "warning: {}", "using defaults");  // to stderr

    // A note on this library: libstdc++ currently provides the FILE* overloads
    // (stdout, stderr, or a fopen'd FILE*), not a std::ostream overload, so pass
    // a FILE* rather than std::cout. (The C++23 standard also specifies an
    // ostream overload; it is not yet available here.)
    std::print(stdout, "done\n");
    return 0;
}
