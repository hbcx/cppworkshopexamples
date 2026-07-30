#include <expected>
#include <iostream>
#include <optional>
#include <string>

enum class LoadError { NotFound, BadFormat };

// Anti-pattern: optional as the return of something that can fail for DIFFERENT
// reasons. nullopt says "no value" but not why, so the caller cannot tell a
// missing source from a malformed one and ends up inventing a side channel.
std::optional<int> loadOptional(const std::string& src) {
    if (src.empty()) return std::nullopt;    // not found
    if (src == "bad") return std::nullopt;   // bad format -- indistinguishable!
    return static_cast<int>(src.size());
}

// Fix: std::expected<T, E> (C++23) carries either the value OR the reason it is
// absent, so the caller can branch on the specific error.
std::expected<int, LoadError> loadExpected(const std::string& src) {
    if (src.empty()) return std::unexpected(LoadError::NotFound);
    if (src == "bad") return std::unexpected(LoadError::BadFormat);
    return static_cast<int>(src.size());
}

const char* name(LoadError e) {
    return e == LoadError::NotFound ? "not found" : "bad format";
}

int main() {
    // optional: both failures look identical.
    for (std::string s : {std::string(""), std::string("bad")}) {
        auto r = loadOptional(s);
        std::cout << "optional(\"" << s << "\"): "
                  << (r ? "ok" : "failed (reason unknown)") << '\n';
    }

    // expected: the reason survives to the caller.
    for (std::string s : {std::string(""), std::string("bad"), std::string("ok")}) {
        auto r = loadExpected(s);
        if (r) std::cout << "expected(\"" << s << "\"): ok, size " << *r << '\n';
        else   std::cout << "expected(\"" << s << "\"): " << name(r.error()) << '\n';
    }
    return 0;
}
