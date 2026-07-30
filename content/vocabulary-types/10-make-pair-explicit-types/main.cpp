#include <iostream>
#include <string>
#include <utility>

int main() {
    int id = 7;
    std::string name = "seven";

    // Anti-pattern: spelling out make_pair's template arguments. It throws away
    // the deduction that is make_pair's whole reason to exist, and it fixes the
    // parameters to rvalue references (int&&, std::string&&), so passing the
    // lvalues id and name does NOT compile. Left as a comment -- it is a hard
    // error by design, not something to run:
    //
    //     auto bad = std::make_pair<int, std::string>(id, name);  // won't bind lvalues
    //
    // With mismatched explicit types it instead forces silent conversions.

    // Fix 1: let make_pair deduce -- the point of the function.
    auto p = std::make_pair(id, name);            // pair<int, std::string>
    std::cout << p.first << " / " << p.second << '\n';

    // Fix 2: when you genuinely want specific member types, name the PAIR type.
    // Its constructor takes the values by reference and converts as needed, with
    // no rvalue-reference surprise.
    std::pair<long, std::string> q(id, name);     // int widened to long, on purpose
    std::cout << q.first << " / " << q.second << '\n';
    return 0;
}
