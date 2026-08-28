#include <iostream>
#include <compare>
#include <string>

// Anti-pattern: letting = default compare a pointer member. The generated
// comparison uses the pointer's ADDRESS, so two labels with identical text but
// separate storage compare as different.

struct BadLabel {
    const std::string* text;   // pointer member
    // Memberwise default: compares the address in `text`, not the string it names.
    auto operator<=>(const BadLabel&) const = default;
    bool operator==(const BadLabel&) const = default;
};

struct GoodLabel {
    const std::string* text;
    // Compare what the pointer points AT.
    std::strong_ordering operator<=>(const GoodLabel& other) const {
        return *text <=> *other.text;
    }
    bool operator==(const GoodLabel& other) const {
        return *text == *other.text;
    }
};

int main() {
    std::string s1{"warning"};
    std::string s2{"warning"};   // same content, different object => different address

    BadLabel ba{&s1}, bb{&s2};
    // Equal text, but the default compares addresses, so they look different:
    std::cout << "bad:  same text, equal? " << (ba == bb) << "\n";   // 0  (wrong)

    GoodLabel ga{&s1}, gb{&s2};
    std::cout << "good: same text, equal? " << (ga == gb) << "\n";   // 1  (compares *text)
    return 0;
}
