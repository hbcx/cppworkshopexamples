#include <iostream>

// The diamond: Scanner and Printer both derive from Device, and Copier inherits
// both. With VIRTUAL inheritance, the shared Device exists once, so copier.id is
// unambiguous and there is a single shared state.

struct Device {
    int id;
    explicit Device(int i) : id(i) {}
};

struct Scanner : virtual Device {          // virtual: share the Device
    Scanner() : Device(1) {}               // bypassed when Scanner is part of a Copier
};

struct Printer : virtual Device {
    Printer() : Device(2) {}               // also bypassed inside a Copier
};

struct Copier : Scanner, Printer {
    // The most-derived class initializes the shared virtual base directly.
    Copier() : Device(42), Scanner(), Printer() {}
};

int main() {
    Copier c;

    // One shared Device: no qualification needed, and both sides see the same id.
    std::cout << "copier.id = " << c.id << "\n";                       // 42

    Scanner& asScanner = c;
    Printer& asPrinter = c;
    std::cout << "as Scanner: " << asScanner.id
              << ", as Printer: " << asPrinter.id << "\n";             // 42, 42

    // They are literally the same subobject:
    std::cout << "same Device? " << (&asScanner.id == &asPrinter.id) << "\n";  // 1
    return 0;
}
