#include <iostream>

// Anti-pattern: a diamond WITHOUT virtual inheritance. Scanner and Printer each
// carry their own Device, so BadCopier ends up with two -- duplicated state, and
// a plain .id that will not compile.

struct Device {
    int id;
    explicit Device(int i) : id(i) {}
};

struct BadScanner : Device { BadScanner() : Device(1) {} };   // non-virtual
struct BadPrinter : Device { BadPrinter() : Device(2) {} };   // non-virtual
struct BadCopier : BadScanner, BadPrinter {};                 // gets TWO Devices

// The fix: virtual inheritance makes Device shared and single.
struct Scanner : virtual Device { Scanner() : Device(1) {} };
struct Printer : virtual Device { Printer() : Device(2) {} };
struct GoodCopier : Scanner, Printer { GoodCopier() : Device(42) {} };

int main() {
    BadCopier bad;
    // bad.id;   // would NOT compile: ambiguous -- which Device?
    // The two copies are independent, and drift apart:
    bad.BadScanner::id = 100;
    bad.BadPrinter::id = 200;
    std::cout << "bad: Scanner::id = " << bad.BadScanner::id
              << ", Printer::id = " << bad.BadPrinter::id << "\n";   // 100, 200 (two states)

    GoodCopier good;
    // One shared Device: plain .id works and there is a single value.
    std::cout << "good: id = " << good.id << "\n";                   // 42
    return 0;
}
