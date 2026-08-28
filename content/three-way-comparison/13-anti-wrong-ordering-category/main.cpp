#include <iostream>
#include <compare>
#include <string>
#include <algorithm>
#include <cctype>

// Anti-pattern: usernames compared with = default. Case should not matter for a
// login name, but the defaulted <=> compares bytes, so "Admin" and "admin" look
// like two different accounts.

struct BadUser {
    std::string name;
    // Byte-for-byte, strong ordering -- wrong rule for a username.
    auto operator<=>(const BadUser&) const = default;
    bool operator==(const BadUser&) const = default;
};

struct GoodUser {
    std::string name;
    // The real rule: case-insensitive, hence weak_ordering (equivalent, not equal).
    std::weak_ordering operator<=>(const GoodUser& other) const {
        const std::size_t n = std::min(name.size(), other.name.size());
        for (std::size_t i = 0; i < n; ++i) {
            const int a = std::tolower(static_cast<unsigned char>(name[i]));
            const int b = std::tolower(static_cast<unsigned char>(other.name[i]));
            if (a != b)
                return a < b ? std::weak_ordering::less : std::weak_ordering::greater;
        }
        if (name.size() != other.name.size())
            return name.size() < other.name.size()
                       ? std::weak_ordering::less : std::weak_ordering::greater;
        return std::weak_ordering::equivalent;
    }
    bool operator==(const GoodUser& other) const { return (*this <=> other) == 0; }
};

int main() {
    // The bug: the same account name registered twice, seen as two users.
    std::cout << "bad:  Admin == admin? "
              << (BadUser{"Admin"} == BadUser{"admin"}) << "\n";   // 0  (wrong)

    std::cout << "good: Admin == admin? "
              << (GoodUser{"Admin"} == GoodUser{"admin"}) << "\n"; // 1  (same account)
    return 0;
}
