// You can make your own error enum act like a standard error_code, so callers
// use one uniform mechanism. Four pieces: an enum, a category (name + message),
// a make_error_code overload, and an is_error_code_enum specialization that
// enables the implicit conversion.
#include <iostream>
#include <string>
#include <system_error>

// 1) the error enum
enum class ConfigError {
    ok = 0,
    missing_key,
    bad_value,
    unreadable
};

// 2) the category: identity, a name, and a message per value (your own text)
class ConfigErrorCategory : public std::error_category {
public:
    const char* name() const noexcept override { return "config"; }
    std::string message(int ev) const override {
        switch (static_cast<ConfigError>(ev)) {
            case ConfigError::ok:          return "no error";
            case ConfigError::missing_key: return "required key is missing";
            case ConfigError::bad_value:   return "value has the wrong format";
            case ConfigError::unreadable:  return "config file cannot be read";
        }
        return "unknown config error";
    }
};

// 3) one global instance; its address is the category's identity
const std::error_category& config_category() {
    static const ConfigErrorCategory instance;
    return instance;
}

// 4a) build an error_code from the enum (found by argument-dependent lookup)
std::error_code make_error_code(ConfigError e) {
    return { static_cast<int>(e), config_category() };
}

// 4b) tell the library this enum names error_code values -> enables conversion
namespace std {
    template <> struct is_error_code_enum<ConfigError> : true_type {};
}

int main() {
    std::cout << std::boolalpha;

    // Implicit conversion from the enum, thanks to step 4:
    std::error_code ec = ConfigError::bad_value;

    std::cout << "category: " << ec.category().name() << '\n';   // config
    std::cout << "message:  " << ec.message() << '\n';           // your text
    std::cout << "is bad_value:   " << (ec == ConfigError::bad_value) << '\n';    // true
    std::cout << "is missing_key: " << (ec == ConfigError::missing_key) << '\n';  // false
    return 0;
}
