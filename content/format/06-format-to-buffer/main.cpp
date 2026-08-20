#include <format>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

int main() {
    // format_to writes through an output iterator, so it appends into a string
    // you already own -- no temporary result string is allocated.
    std::string out;
    std::format_to(std::back_inserter(out), "id={:04}", 7);
    std::format_to(std::back_inserter(out), " name={}", "gadget");
    std::cout << out << '\n';                         // id=0007 name=gadget

    // formatted_size reports how many characters the result would need.
    std::size_t need = std::formatted_size("{:.3f}", 3.14159);
    std::cout << "needs " << need << " chars\n";      // 5

    // format_to_n writes at MOST n characters into a fixed buffer and reports how
    // many it WOULD have written -- so you can detect truncation.
    std::vector<char> buf(6);
    auto res = std::format_to_n(buf.data(), buf.size(), "{}", 1234567);
    std::cout << "wrote " << (res.out - buf.data()) << " of " << res.size
              << " chars: " << std::string(buf.data(), res.out) << '\n';   // 6 of 7: 123456
    return 0;
}
