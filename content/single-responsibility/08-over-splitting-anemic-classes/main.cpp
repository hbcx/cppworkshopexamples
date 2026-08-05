// Anti-pattern: over-splitting into anemic classes.
//
// SRP read as "one class, one method" shatters a single job into a crowd of
// stateless one-line classes. Below, four -er classes each do one trivial step
// of ONE responsibility -- normalize a name -- and a coordinator wires them.
// The work is buried under object plumbing, and none of it is more testable
// than a plain function would be.
//
// The fix merges by reason to change: they all change when "a valid name"
// changes, so they are one cohesive NameNormalizer.

#include <cctype>
#include <iostream>
#include <string>

// ---------- one class per micro-step ----------
namespace bad {

class NameTrimmer {
public:
    std::string trim(const std::string& s) const {
        std::size_t a = s.find_first_not_of(' ');
        std::size_t b = s.find_last_not_of(' ');
        if (a == std::string::npos) return "";
        return s.substr(a, b - a + 1);
    }
};

class NameLowercaser {
public:
    std::string lower(std::string s) const {
        for (char& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        return s;
    }
};

class NameCapitalizer {
public:
    std::string capitalize(std::string s) const {
        if (!s.empty()) s[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(s[0])));
        return s;
    }
};

class NameValidator {
public:
    bool valid(const std::string& s) const { return !s.empty(); }
};

// More wiring than logic.
class NameProcessor {
public:
    std::string process(const std::string& raw) const {
        NameTrimmer trimmer;
        NameLowercaser lowerer;
        NameCapitalizer capitalizer;
        NameValidator validator;
        std::string s = capitalizer.capitalize(lowerer.lower(trimmer.trim(raw)));
        return validator.valid(s) ? s : "(invalid)";
    }
};

} // namespace bad

// ---------- one cohesive class ----------
namespace good {

// One responsibility -- normalize a name -- expressed as internal steps. When
// "a valid name" changes, exactly one class changes.
class NameNormalizer {
public:
    std::string normalize(const std::string& raw) const {
        std::string s = trim(raw);
        for (char& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        if (!s.empty()) s[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(s[0])));
        return s.empty() ? "(invalid)" : s;
    }

private:
    static std::string trim(const std::string& s) {
        std::size_t a = s.find_first_not_of(' ');
        std::size_t b = s.find_last_not_of(' ');
        if (a == std::string::npos) return "";
        return s.substr(a, b - a + 1);
    }
};

} // namespace good

int main() {
    const std::string raw = "   aDA lovelace  ";

    bad::NameProcessor processor;
    std::cout << "[bad]  four classes: '" << processor.process(raw) << "'\n";

    good::NameNormalizer normalizer;
    std::cout << "[good] one class:    '" << normalizer.normalize(raw) << "'\n";
}
