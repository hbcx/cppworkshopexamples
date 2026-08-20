#include <format>
#include <iostream>
#include <locale>
#include <string>

// A small locale facet that groups thousands with commas, so the example is
// deterministic instead of depending on which locales the machine has installed.
struct CommaGroup : std::numpunct<char> {
    char do_thousands_sep() const override { return ','; }
    std::string do_grouping() const override { return "\3"; }
};

int main() {
    // By DEFAULT std::format is locale-INDEPENDENT: no grouping, a '.' decimal.
    // That is what machine-read output (logs, protocols, CSV) needs.
    std::cout << std::format("default:     {}\n", 1234567);      // 1234567
    std::cout << std::format("default:     {:.2f}\n", 1234.5);   // 1234.50

    // The L option opts a number INTO a locale's grouping rules. Pass the locale
    // explicitly so you never depend on the process-wide global one.
    std::locale grouped(std::locale::classic(), new CommaGroup);
    std::cout << std::format(grouped, "grouped (L): {:L}\n", 1234567);            // 1,234,567
    std::cout << std::format(std::locale::classic(), "classic (L): {:L}\n", 1234567); // 1234567
    return 0;
}
