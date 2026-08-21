#include <charconv>
#include <iostream>
#include <string_view>
#include <system_error>

int main() {
    // from_chars reports two different failures through ec:
    //   invalid_argument     -> the text did not start with a number at all
    //   result_out_of_range  -> a number was there but does not fit the type
    auto classify = [](std::string_view s) {
        int v = 0;
        auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), v);
        (void)ptr;
        std::cout << "\"" << s << "\" -> ";
        if (ec == std::errc{})
            std::cout << "ok: " << v << '\n';
        else if (ec == std::errc::invalid_argument)
            std::cout << "invalid_argument (no number)\n";
        else if (ec == std::errc::result_out_of_range)
            std::cout << "result_out_of_range (overflow)\n";
    };

    classify("42");
    classify("xyz");                     // invalid_argument
    classify("99999999999999999999");    // result_out_of_range for int

    // to_chars fails the other way: too small a buffer gives value_too_large,
    // and it writes NOTHING usable -- so check ec before reading the buffer.
    char tiny[3];
    auto [ptr, ec] = std::to_chars(tiny, tiny + sizeof tiny, 123456);
    (void)ptr;
    std::cout << "to_chars into a 3-byte buffer: "
              << (ec == std::errc::value_too_large ? "value_too_large" : "ok")
              << '\n';
    return 0;
}
