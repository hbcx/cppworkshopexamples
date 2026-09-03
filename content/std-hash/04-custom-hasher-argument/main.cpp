#include <cctype>
#include <cstddef>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

// A hasher for case-insensitive string keys. It is a plain function object; we do
// NOT specialize std::hash<std::string>, because that would change hashing for
// EVERY std::string in the whole program.
struct CaseInsensitiveHash {
    std::size_t operator()(const std::string& s) const {
        std::size_t seed = 0;
        for (char c : s) {
            char lower = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
            seed ^= std::hash<char>{}(lower) + 0x9e3779b9u + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

struct CaseInsensitiveEq {
    bool operator()(const std::string& a, const std::string& b) const {
        if (a.size() != b.size()) return false;
        for (std::size_t i = 0; i < a.size(); ++i) {
            if (std::tolower(static_cast<unsigned char>(a[i])) !=
                std::tolower(static_cast<unsigned char>(b[i]))) return false;
        }
        return true;
    }
};

int main() {
    // Pass the hasher (and a matching equality) as template arguments. Nothing in
    // namespace std is touched, and the special hashing is scoped to THIS map.
    std::unordered_map<std::string, int, CaseInsensitiveHash, CaseInsensitiveEq> counts;
    counts["Hello"] += 1;
    counts["HELLO"] += 1;
    counts["hello"] += 1;

    std::cout << "distinct keys: " << counts.size() << "\n";         // 1
    std::cout << "count for 'HeLLo': " << counts["HeLLo"] << "\n";   // 3

    // Prefer this when you do NOT own the key type, or want context-specific
    // hashing; specialize std::hash only for the ONE natural hash of a type you own.
    return 0;
}
