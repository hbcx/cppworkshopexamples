// Anti-pattern: deciding what to do by matching error_code::message() text. The
// message is human-readable, locale-dependent and platform-specific -- the same
// error reads "No such file or directory", a translated phrase, or a Windows
// wording. Matching on it breaks across locales and systems. Compare the code
// against a std::errc value instead; the text is for humans, the errc is for code.
#include <iostream>
#include <string>
#include <system_error>

// Decide whether to retry, the WRONG way: substring-match the message.
bool should_retry_wrong(const std::error_code& ec) {
    return ec.message().find("temporarily") != std::string::npos;   // fragile
}

// The RIGHT way: match the meaning, portably.
bool should_retry_right(const std::error_code& ec) {
    return ec == std::errc::resource_unavailable_try_again
        || ec == std::errc::timed_out;
}

int main() {
    std::cout << std::boolalpha;
    std::error_code ec = std::make_error_code(std::errc::timed_out);

    // The text match may be true or false depending on how THIS platform words
    // the message -- we do not even print it, because it is not portable.
    (void)should_retry_wrong(ec);

    std::cout << "retry (by errc): " << should_retry_right(ec) << '\n';   // true
    return 0;
}
