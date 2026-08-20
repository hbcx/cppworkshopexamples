#include <format>
#include <iostream>
#include <locale>
#include <string>

// A locale facet that groups thousands with commas (deterministic for the test).
struct CommaGroup : std::numpunct<char> {
    char do_thousands_sep() const override { return ','; }
    std::string do_grouping() const override { return "\3"; }
};

int main() {
    long amount = 1234567;

    // WRONG for machine-readable output: L (or formatting under a grouping locale)
    // inserts thousands separators, so a CSV field becomes 1,234,567 -- which a
    // parser reads as THREE fields, silently corrupting the row.
    std::locale grouped(std::locale::classic(), new CommaGroup);
    std::string csvBad = std::format(grouped, "id,amount\n1,{:L}\n", amount);
    std::cout << "BAD csv:\n" << csvBad;

    // RIGHT: machine output is locale-INDEPENDENT. Plain {} (no L) always uses a
    // '.' decimal and no grouping, so the number stays a single field.
    std::string csvGood = std::format("id,amount\n1,{}\n", amount);
    std::cout << "GOOD csv:\n" << csvGood;

    // Keep L for HUMAN-facing text only -- a label or a report a person reads.
    return 0;
}
