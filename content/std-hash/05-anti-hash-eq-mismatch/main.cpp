#include <cstddef>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_set>

// Identity is the id ALONE.
struct User {
    int id;
    std::string name;   // display name, NOT part of identity
};
bool operator==(const User& a, const User& b) { return a.id == b.id; }

// ANTI-PATTERN: this hash mixes in `name`, a field operator== ignores. Two Users
// that are EQUAL (same id, different name) now hash differently, breaking the one
// rule every hash must obey: a == b implies hash(a) == hash(b).
struct BadHash {
    std::size_t operator()(const User& u) const {
        return std::hash<int>{}(u.id) ^ std::hash<std::string>{}(u.name);
    }
};

// FIX: hash EXACTLY the fields that define equality -- here, id only.
struct GoodHash {
    std::size_t operator()(const User& u) const { return std::hash<int>{}(u.id); }
};

int main() {
    std::cout << std::boolalpha;
    User a{1, "Ada"};
    User b{1, "Ada Lovelace"};

    // a and b are equal, but the bad hash disagrees -- the invariant is broken.
    std::cout << "a == b?               " << (a == b) << "\n";                        // true
    std::cout << "BadHash agrees?       " << (BadHash{}(a) == BadHash{}(b)) << "\n";  // false -- BUG
    std::cout << "GoodHash agrees?      " << (GoodHash{}(a) == GoodHash{}(b)) << "\n"; // true

    // With the broken hash the two equal Users can land in different buckets, so a
    // lookup for an element that IS present may miss. The correct hash makes the
    // lookup reliable, because equal keys always hash the same:
    std::unordered_set<User, GoodHash> users;
    users.insert(a);
    std::cout << "found b via GoodHash? " << (users.count(b) != 0) << "\n";           // true
    return 0;
}
