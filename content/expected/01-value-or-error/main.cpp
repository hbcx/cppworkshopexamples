#include <expected>
#include <iostream>
#include <string>

int main() {
    // expected<T, E> holds EITHER a value (T) or an error (E). Build it from a
    // value for success, or from std::unexpected(e) for failure.
    std::expected<int, std::string> ok = 42;
    std::expected<int, std::string> bad = std::unexpected("disk full");

    std::cout << std::boolalpha;
    // has_value() -- or the bool conversion -- says which side it holds.
    std::cout << "ok.has_value() = " << ok.has_value() << '\n';
    std::cout << "bad (as bool)  = " << static_cast<bool>(bad) << '\n';

    // On success, read the value with * or ->. On failure, read error().
    // Each is valid only on its own side (see 03).
    if (ok) std::cout << "value = " << *ok << '\n';
    if (!bad) std::cout << "error = " << bad.error() << '\n';

    std::expected<std::string, int> name = "Ada";
    std::cout << "length via -> = " << name->size() << '\n';
    return 0;
}
