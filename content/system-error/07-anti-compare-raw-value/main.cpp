// Anti-pattern: comparing an error_code by its raw value() (or against a
// hard-coded number). The number only has meaning together with its category --
// the same value means different things in different categories, and a
// platform-specific category uses different numbers. Compare against std::errc,
// which matches by MEANING across categories.
#include <iostream>
#include <string>
#include <system_error>

// A tiny custom category, so we can build a code whose number collides with a
// POSIX one but means something unrelated.
class DeviceCategory : public std::error_category {
public:
    const char* name() const noexcept override { return "device"; }
    std::string message(int) const override { return "device error"; }
};
const std::error_category& device_category() {
    static const DeviceCategory c;
    return c;
}

int main() {
    std::cout << std::boolalpha;

    std::error_code posix = std::make_error_code(std::errc::no_such_file_or_directory);
    // Deliberately reuse posix's number in a different category.
    std::error_code device(posix.value(), device_category());

    // WRONG: raw value() ignores the category -- two unrelated errors look equal.
    std::cout << "same raw value: " << (posix.value() == device.value()) << '\n';   // true
    // ...but they are NOT the same error:
    std::cout << "actually equal: " << (posix == device) << '\n';                   // false

    // RIGHT: compare against the meaning, via errc. Portable and category-safe.
    std::cout << "posix is enoent:  " << (posix == std::errc::no_such_file_or_directory) << '\n';  // true
    std::cout << "device is enoent: " << (device == std::errc::no_such_file_or_directory) << '\n'; // false
    return 0;
}
