#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>
#include <string>

// A custom facet: comma decimal point and a grouping dot every three digits.
// Used instead of a named system locale (like "de_DE") so the result is exactly
// the same on every machine, whether or not that locale is installed.
struct CommaPunct : std::numpunct<char> {
    char do_decimal_point() const override { return ','; }
    char do_thousands_sep() const override { return '.'; }
    std::string do_grouping() const override { return "\3"; }
};

int main() {
    // Anti-pattern: assuming a stringstream formats numbers the same everywhere.
    // A stream uses a LOCALE, and imbuing one changes the decimal separator and
    // adds digit grouping -- so the "same" code produces different text, and
    // text written under one locale can be misread under another.
    double value = 1234.5;

    // Default (classic "C") locale: dot decimal, no grouping.
    std::ostringstream classic;
    classic << std::fixed << std::setprecision(1) << value;
    std::cout << "classic: " << classic.str() << '\n';   // 1234.5

    // Imbue a comma/grouping locale: same value, different text.
    std::ostringstream euro;
    euro.imbue(std::locale(std::locale::classic(), new CommaPunct));
    euro << std::fixed << std::setprecision(1) << value;
    std::cout << "euro:    " << euro.str() << '\n';       // 1.234,5

    // On the READ side the same locale would misparse: under this facet the '.'
    // in "1234.5" is a GROUPING separator, not a decimal point, so a reader
    // expecting a dot decimal gets the wrong number. Fix: pin std::locale::classic()
    // for stored/exchanged data, or use std::to_chars/std::from_chars, which
    // ignore the locale entirely.
    return 0;
}
