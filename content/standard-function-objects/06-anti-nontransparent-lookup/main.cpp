#include <functional>
#include <iostream>
#include <set>
#include <string>

// A key type that counts every construction, so the hidden temporaries a lookup
// builds become visible.
struct CountedString {
    std::string value;
    static int constructions;
    CountedString(const char* s) : value(s) { ++constructions; }
    bool operator<(const CountedString& o) const { return value < o.value; }
};
int CountedString::constructions = 0;

// Heterogeneous comparisons so a transparent comparator can compare a
// CountedString directly against a const char*, constructing no CountedString.
bool operator<(const CountedString& a, const char* b) { return a.value < b; }
bool operator<(const char* a, const CountedString& b) { return a < b.value; }

int main() {
    // ANTI-PATTERN: an ordered container keyed by a class type, looked up by a
    // cheaper type. With the DEFAULT comparator (std::less<CountedString>), find
    // takes a const CountedString&, so every lookup by a literal CONSTRUCTS a
    // temporary key -- a hidden cost (here a std::string; in real code an
    // allocation) repeated on the hot path.
    std::set<CountedString> plain{"alice", "bob", "carol"};
    int before = CountedString::constructions;
    for (int i = 0; i < 3; ++i) plain.find("bob");   // builds a temporary each time
    std::cout << "plain set: " << (CountedString::constructions - before)
              << " temp keys for 3 lookups\n";   // 3

    // FIX: a transparent comparator (std::less<>) plus heterogeneous operator<
    // lets find compare the literal against the keys directly -- no temporary.
    std::set<CountedString, std::less<>> smart{"alice", "bob", "carol"};
    before = CountedString::constructions;
    for (int i = 0; i < 3; ++i) smart.find("bob");   // compares const char* to keys
    std::cout << "smart set: " << (CountedString::constructions - before)
              << " temp keys for 3 lookups\n";   // 0
    return 0;
}
