#include <format>
#include <print>
#include <string>

int main() {
    // Anti-pattern: passing a RUNTIME string as the format. std::print's format
    // parameter is checked at COMPILE time, so it must be a constant expression.
    // A std::string built at runtime does not compile:
    //
    //   std::string fmt = load_format_from_config();
    //   std::print(fmt, 42);        // ERROR: not a compile-time format string
    //
    // Fix (C++23): go through the vformat / vprint path, which takes a runtime
    // format string and type-erased args. (C++26 adds std::runtime_format.)
    std::string fmt = "value = {}, hex = {:#x}";   // decided at runtime
    int v = 255;                                    // make_format_args needs lvalues

    // vformat builds a string from the runtime format, then we print it.
    std::string out = std::vformat(fmt, std::make_format_args(v, v));
    std::println("{}", out);        // value = 255, hex = 0xff

    // Or write it straight to stdout with vprint_unicode (no intermediate string).
    std::vprint_unicode(stdout, fmt, std::make_format_args(v, v));
    std::print("\n");
    return 0;
}
