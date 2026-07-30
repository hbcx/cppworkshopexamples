#include <expected>
#include <iostream>
#include <string>

int main() {
    std::expected<int, std::string> ok = 10;
    std::expected<int, std::string> bad = std::unexpected("nope");

    // value_or returns the value, or the fallback when it holds an error.
    std::cout << "ok.value_or(-1)  = " << ok.value_or(-1) << '\n';   // 10
    std::cout << "bad.value_or(-1) = " << bad.value_or(-1) << '\n';  // -1

    // value() returns the value but THROWS std::bad_expected_access if it holds
    // an error -- and the exception CARRIES the error object via its .error().
    try {
        (void) bad.value();
    } catch (const std::bad_expected_access<std::string>& e) {
        std::cout << "value() threw, carrying error: " << e.error() << '\n';
    }

    // *, -> and error() are UNCHECKED. Reading the value of an error, or the
    // error of a value, is undefined behaviour -- so guard first:
    //     *bad;         // UB: no value on the error side
    //     ok.error();   // UB: no error on the value side
    std::cout << "*ok = " << *ok << '\n';
    return 0;
}
