#include <iostream>
#include <compare>
#include <string>
#include <algorithm>
#include <cctype>

// A name compared without regard to case. Two names that differ only in case are
// EQUIVALENT but not identical, which is exactly what weak_ordering expresses.

struct CIName {
    std::string value;

    std::weak_ordering operator<=>(const CIName& other) const {
        const std::size_t n = std::min(value.size(), other.value.size());
        for (std::size_t i = 0; i < n; ++i) {
            const int a = std::tolower(static_cast<unsigned char>(value[i]));
            const int b = std::tolower(static_cast<unsigned char>(other.value[i]));
            if (a != b)
                return a < b ? std::weak_ordering::less : std::weak_ordering::greater;
        }
        // Shared prefix matches; the shorter name comes first.
        if (value.size() != other.value.size())
            return value.size() < other.value.size()
                       ? std::weak_ordering::less : std::weak_ordering::greater;
        return std::weak_ordering::equivalent;
    }

    bool operator==(const CIName& other) const {
        return (*this <=> other) == 0;   // equivalent under the case-insensitive order
    }
};

int main() {
    CIName a{"Hello"};
    CIName b{"hello"};

    // Equivalent for both ordering and equality...
    std::cout << "Hello == hello? " << (a == b) << "\n";   // 1
    std::cout << "Hello <  hello? " << (a < b)  << "\n";   // 0  (neither, they are equivalent)

    // ...yet the stored characters are genuinely different -- weak, not strong:
    std::cout << "but stored values differ? " << (a.value != b.value) << "\n";  // 1

    CIName apple{"apple"};
    CIName banana{"Banana"};
    std::cout << "apple < Banana? " << (apple < banana) << "\n";   // 1  (a before b, case aside)
    return 0;
}
