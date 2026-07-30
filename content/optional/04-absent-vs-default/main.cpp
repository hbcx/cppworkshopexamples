#include <iostream>
#include <optional>

// A config where a timeout may be UNSET, which is different from 0 (disabled).
struct Config {
    std::optional<int> timeoutSeconds;   // nullopt = "use the built-in default"
};

int effectiveTimeout(const Config& c) {
    // Fall back only when the field is genuinely absent; an explicit 0 stays 0.
    return c.timeoutSeconds.value_or(30);
}

int main() {
    Config unset;                        // timeout not mentioned at all
    Config disabled;
    disabled.timeoutSeconds = 0;         // explicitly zero -- a real choice
    Config custom;
    custom.timeoutSeconds = 5;

    std::cout << "unset    -> " << effectiveTimeout(unset) << '\n';     // 30 (default)
    std::cout << "disabled -> " << effectiveTimeout(disabled) << '\n';  // 0 (kept!)
    std::cout << "custom   -> " << effectiveTimeout(custom) << '\n';    // 5

    // A plain int could not tell "unset" from 0 without a magic sentinel;
    // optional makes absence a distinct, honest state.
    return 0;
}
