// Anti-pattern: business logic tangled with I/O.
//
// The bad ReportService fetches rows, sums them, and writes the report in ONE
// method. The arithmetic cannot be tested on its own -- to check the total you
// must run the fetch and capture the write. The interesting cases (empty input,
// a rounding edge) are painful to reach through real I/O.
//
// The fix extracts a pure summarise(): rows in, result out, no reading or
// writing. Fetch and presentation stay at the edges.

#include <iostream>
#include <string>
#include <vector>

struct Sale {
    std::string region;
    long amountCents;
};

// A stand-in for a real source (database, file, socket).
static std::vector<Sale> fetchSales() {
    return {{"North", 12000}, {"South", 8050}, {"East", 4300}};
}

// ---------- logic welded to I/O ----------
namespace bad {

class ReportService {
public:
    // Fetch + compute + present, all in one place. To test the SUM you must
    // fetch real rows and read the printed line back.
    void generateReport() {
        std::vector<Sale> rows = fetchSales();          // input
        long total = 0;
        int count = 0;
        for (const Sale& s : rows) { total += s.amountCents; ++count; } // logic
        std::cout << "[bad] report: " << count << " sales, $"           // output
                  << total / 100 << "." << (total % 100 < 10 ? "0" : "")
                  << total % 100 << "\n";
    }
};

} // namespace bad

// ---------- pure core, I/O at the edges ----------
namespace good {

struct Summary {
    int count;
    long totalCents;
};

// Pure: no fetch, no print. Testable with a handful of rows in memory.
class SalesSummariser {
public:
    Summary summarise(const std::vector<Sale>& rows) const {
        Summary s{0, 0};
        for (const Sale& r : rows) { s.totalCents += r.amountCents; ++s.count; }
        return s;
    }
};

// Presentation lives on its own and can be swapped for a file or a web view.
class ReportPrinter {
public:
    void print(const Summary& s) const {
        std::cout << "[good] report: " << s.count << " sales, $"
                  << s.totalCents / 100 << "."
                  << (s.totalCents % 100 < 10 ? "0" : "") << s.totalCents % 100
                  << "\n";
    }
};

} // namespace good

int main() {
    bad::ReportService legacy;
    legacy.generateReport();

    std::cout << "---\n";

    good::SalesSummariser summariser;

    // The pure core is unit-testable with NO I/O -- including the empty case,
    // which was awkward to reach through a real source.
    good::Summary empty = summariser.summarise({});
    std::cout << "self-test empty: "
              << (empty.count == 0 && empty.totalCents == 0 ? "ok" : "FAIL")
              << "\n";

    good::Summary s = summariser.summarise(fetchSales());
    good::ReportPrinter printer;
    printer.print(s);
}
