#include <format>
#include <iostream>
#include <string>

struct Temp { double celsius; };

// WRONG (described): a formatter whose parse() returns begin() without scanning
// the spec. The returned iterator is not left on the closing brace, so any caller
// that writes a spec like {:.1f} FAILS TO COMPILE -- it is not a silent bug, it
// breaks every use of a spec:
//   template <> struct std::formatter<Temp> {
//       constexpr auto parse(std::format_parse_context& c) { return c.begin(); }
//       auto format(const Temp& t, std::format_context& c) const {
//           return std::format_to(c.out(), "{}C", t.celsius);
//       }
//   };
//   std::format("{:.1f}", Temp{21.5});     // does NOT compile

// RIGHT: inherit from an existing formatter so ITS parse() consumes the spec,
// then reuse its format() for the number and append your own suffix.
template <>
struct std::formatter<Temp> : std::formatter<double> {
    auto format(const Temp& t, std::format_context& ctx) const {
        auto out = std::formatter<double>::format(t.celsius, ctx);
        return std::format_to(out, "C");
    }
};

int main() {
    Temp t{21.567};
    std::cout << std::format("no spec : {}\n", t);          // 21.567C
    std::cout << std::format("with .1f: {:.1f}\n", t);      // 21.6C -- spec honored
    std::cout << std::format("width   : [{:>10.1f}]\n", t); // number aligned in width 10
    return 0;
}
