#include <format>
#include <iostream>
#include <string>

struct Point { int x; int y; };

// Teach std::format about Point by specializing std::formatter<Point>.
template <>
struct std::formatter<Point> {
    // parse() reads the spec between the colon and the closing brace. We accept
    // only an empty spec, so we return the iterator we were given unchanged.
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }
    // format() writes the value through the context's output iterator. It must
    // be const.
    auto format(const Point& p, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "({}, {})", p.x, p.y);
    }
};

int main() {
    Point p{3, 4};
    std::cout << std::format("point = {}\n", p);          // point = (3, 4)

    // Once the formatter exists, Point works anywhere std::format does.
    std::cout << std::format("line from {} to {}\n", Point{0, 0}, Point{2, 5});
    return 0;
}
