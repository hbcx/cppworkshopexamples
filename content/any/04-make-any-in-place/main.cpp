#include <any>
#include <iostream>
#include <string>
#include <utility>   // std::in_place_type

struct Widget {
    Widget(int a, int b) { std::cout << "  Widget(" << a << ", " << b << ")\n"; }
};

int main() {
    // make_any<T>(args...) builds an any holding a T constructed from args --
    // the any counterpart to make_optional / make_shared.
    auto a = std::make_any<std::string>(5, 'x');   // string(5, 'x') = "xxxxx"
    std::cout << "make_any string = " << std::any_cast<std::string>(a) << '\n';

    // The in_place_type constructor builds the Widget directly inside the any
    // from the given arguments -- no temporary Widget created and moved in.
    std::cout << "in_place_type:\n";
    std::any w(std::in_place_type<Widget>, 1, 2);
    (void) w;
    return 0;
}
