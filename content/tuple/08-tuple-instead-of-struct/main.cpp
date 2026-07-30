#include <iostream>
#include <string>
#include <tuple>

// Anti-pattern: a tuple used as a domain type. Position carries the meaning, so
// get<2> tells a reader nothing, and two same-typed fields (width, height) can
// be transposed with no complaint from the compiler.
std::tuple<std::string, int, int> makeBoxBad() {
    // (label, width, height) or (label, height, width)? Only this line knows.
    return std::make_tuple("banner", 1920, 1080);
}

// Fix: a struct with named fields. Same layout and cost; the names document the
// data at every use site, and get<N> becomes real member access.
struct Box {
    std::string label;
    int width;
    int height;
};

Box makeBoxGood() {
    return {"banner", 1920, 1080};
}

int main() {
    auto bad = makeBoxBad();
    // Is get<1> the width or the height? You must go read makeBoxBad to know.
    std::cout << std::get<0>(bad) << ": " << std::get<1>(bad)
              << "x" << std::get<2>(bad) << '\n';

    Box good = makeBoxGood();
    std::cout << good.label << ": " << good.width << "x" << good.height << '\n';
    return 0;
}
