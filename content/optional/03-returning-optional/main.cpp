#include <iostream>
#include <optional>
#include <string>
#include <vector>

// A lookup that may fail returns optional<T> instead of a magic sentinel or a
// bool-plus-out-parameter. "No result" is now part of the type.
std::optional<int> parsePort(const std::string& s) {
    if (s.empty()) return std::nullopt;
    int value = 0;
    for (char c : s) {
        if (c < '0' || c > '9') return std::nullopt;   // not a number
        value = value * 10 + (c - '0');
    }
    return value;   // implicit conversion T -> optional<T>
}

int main() {
    std::vector<std::string> inputs = {"8080", "http", ""};
    for (const std::string& in : inputs) {
        std::optional<int> port = parsePort(in);
        if (port) std::cout << "\"" << in << "\" -> port " << *port << '\n';
        else      std::cout << "\"" << in << "\" -> not a valid port\n";
    }
    return 0;
}
