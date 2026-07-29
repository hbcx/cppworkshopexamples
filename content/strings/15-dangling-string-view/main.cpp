#include <iostream>
#include <string>
#include <string_view>

// WRONG (described, not run): returns a view of a local string that is destroyed
// when the function returns, so the caller reads freed memory.
//   std::string_view labelBad() {
//       std::string s = "id-" + std::to_string(42);
//       return s;                 // s dies here; the returned view dangles
//   }

// RIGHT: return an owning std::string when the data is produced locally.
static std::string labelGood() {
    return "id-" + std::to_string(42);
}

// RIGHT: a view parameter is fine when the caller's argument outlives the call.
static std::size_t viewLength(std::string_view sv) {
    return sv.size();
}

int main() {
    // WRONG (described, not run): the concatenation is a temporary destroyed at
    // the semicolon, so the view dangles on the very next line.
    //   std::string_view bad = std::string("a") + "b";
    //   std::cout << bad;         // undefined behaviour

    std::string label = labelGood();          // owns its characters
    std::cout << "label = " << label << '\n';

    // Safe view: it refers to `label`, which stays alive for the whole use.
    std::string_view sv = label;
    std::cout << "view size = " << viewLength(sv) << '\n';

    // Safe view of a literal: literals live for the whole program.
    std::cout << "literal view size = " << viewLength("literals never dangle") << '\n';
    return 0;
}
