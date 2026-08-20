#include <iostream>
#include <string>
#include <string_view>

// WRONG (described): a type that STORES a view borrows the caller's buffer for
// its whole lifetime. If the source dies first, every later read is a dangling
// read -- and it compiles without a warning.
//   struct LabelBad {
//       std::string_view text;                       // borrows, does not own
//       explicit LabelBad(std::string_view t) : text(t) {}
//   };
//   LabelBad make() {
//       std::string local = "built here";
//       return LabelBad(local);                       // local dies -> text dangles
//   }

// RIGHT: a value that must OUTLIVE the call has to be OWNED. Store a std::string;
// take the argument as a view so callers still pay nothing to pass one in.
struct Label {
    std::string text;                                  // owns its characters
    explicit Label(std::string_view t) : text(t) {}    // copy in, once
};

int main() {
    Label a("built here");            // the copy lives inside the object -- safe
    std::cout << "a = " << a.text << '\n';

    Label b = [] {
        std::string local = "made in a lambda";
        return Label(local);          // local dies, but Label already copied it
    }();
    std::cout << "b = " << b.text << '\n';

    // Rule: a view is for BORROWING across a call, never for KEEPING in an object.
    return 0;
}
