#include <iostream>
#include <map>
#include <string>
#include <tuple>

std::string makeName() { return "temporary"; }

int main() {
    // std::forward_as_tuple builds a tuple of REFERENCES to its arguments, to
    // perfect-forward a set of args straight into a call. It owns nothing, so it
    // must be consumed in the same full expression it is created in.
    //
    // Anti-pattern: storing it and using it later. The temporaries it referred
    // to are already gone, so the references dangle. Not run -- UB is
    // nondeterministic and would break the two-compiler output check:
    //
    //     auto bad = std::forward_as_tuple(makeName(), 42);
    //     std::cout << std::get<0>(bad);   // UB: makeName()'s result destroyed
    //
    // The reference nature is invisible behind auto -- it looks like it copied.

    // Correct use: forward into a call in the same expression. That is what
    // forward_as_tuple is for -- here, building a map value in place with
    // piecewise_construct; the argument tuples are consumed immediately.
    std::map<int, std::string> m;
    m.emplace(std::piecewise_construct,
              std::forward_as_tuple(1),
              std::forward_as_tuple(3, 'a'));   // value = string(3, 'a') = "aaa"
    std::cout << "emplaced key 1 -> " << m.at(1) << '\n';

    // To KEEP a tuple beyond the expression, use make_tuple: it stores by value
    // (owns copies), so it safely outlives the arguments.
    auto kept = std::make_tuple(makeName(), 42);
    std::cout << "kept: " << std::get<0>(kept) << " / " << std::get<1>(kept) << '\n';
    return 0;
}
