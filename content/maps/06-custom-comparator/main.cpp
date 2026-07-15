#include <cctype>
#include <cstddef>
#include <iostream>
#include <map>
#include <string>

// Orders strings case-insensitively. The map treats two keys as equal when
// neither compares less than the other, so "Apple" and "apple" become one key.
struct CaseInsensitiveLess {
    bool operator()(const std::string& a, const std::string& b) const {
        for (std::size_t i = 0; i < a.size() && i < b.size(); ++i) {
            int la = std::tolower(static_cast<unsigned char>(a[i]));
            int lb = std::tolower(static_cast<unsigned char>(b[i]));
            if (la != lb) {
                return la < lb;
            }
        }
        return a.size() < b.size();
    }
};

int main() {
    std::map<std::string, int, CaseInsensitiveLess> m;
    m["Apple"] = 1;
    m["banana"] = 2;
    m["apple"] = 3;   // same key as "Apple" under our comparator -> updates it

    std::cout << "size = " << m.size() << " (Apple and apple merged)\n";
    for (const auto& entry : m) {
        std::cout << "  " << entry.first << " = " << entry.second << '\n';
    }
    // A lookup with yet another casing finds the merged entry.
    std::cout << "lookup APPLE = " << m["APPLE"] << '\n';
    std::cout << "size still " << m.size() << " after the APPLE lookup\n";
    return 0;
}
