// Anti-pattern: calling a non-throwing, error_code-reporting function and then
// ignoring the code -- using the result as if it succeeded. The error_code
// overload does NOT throw, so nothing stops you; on failure the return value is
// a sentinel or the out-parameter is left unspecified, and skipping the check
// means you compute on garbage. Always inspect the code before trusting the
// result.
#include <iostream>
#include <string>
#include <system_error>

// A parse in the error_code style: on success it sets out and clears ec; on
// failure it sets ec and leaves out untouched.
void parse_port(const std::string& text, int& out, std::error_code& ec) {
    ec.clear();
    if (text.empty()) { ec = std::make_error_code(std::errc::invalid_argument); return; }
    for (char c : text) {
        if (c < '0' || c > '9') {
            ec = std::make_error_code(std::errc::invalid_argument);
            return;
        }
    }
    out = std::stoi(text);
}

int main() {
    std::cout << std::boolalpha;

    // WRONG: ignore ec and use out. out was never set, so this reads a stale value.
    {
        std::error_code ec;
        int port = -1;                     // never checked
        parse_port("bad", port, ec);       // fails, but we do not look at ec
        std::cout << "wrong: used port " << port << " (stale, ec ignored)\n";
    }

    // RIGHT: check ec first; only trust out when the call succeeded.
    {
        std::error_code ec;
        int port = 0;
        parse_port("bad", port, ec);
        if (ec) {
            std::cout << "right: parse failed, is invalid_argument: "
                      << (ec == std::errc::invalid_argument) << '\n';
        } else {
            std::cout << "right: port " << port << '\n';
        }
    }
    return 0;
}
