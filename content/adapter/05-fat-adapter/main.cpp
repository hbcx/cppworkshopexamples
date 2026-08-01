#include <iostream>
#include <algorithm>

struct Vendor {
    double readF() const { return 266.0; }   // 130 C
};

struct Thermometer {
    virtual double celsius() const = 0;
    virtual ~Thermometer() = default;
};

// Anti-pattern: the adapter translates AND applies a business rule (clamp to a
// "safe" 100 C). The rule is baked into the reading, so any consumer that needs
// the TRUE temperature gets the clamped one -- the real 130 is lost, and this
// duplicated rule will drift from the real policy.
struct FatAdapter : Thermometer {
    Vendor v;
    double celsius() const override {
        double c = (v.readF() - 32.0) * 5.0 / 9.0;
        return std::min(c, 100.0);   // business logic inside the adapter
    }
};

// Fix: the adapter only translates -- it returns the true Celsius. The clamp is
// a separate policy applied where it belongs, so the raw reading stays available
// to every other consumer.
struct ThinAdapter : Thermometer {
    Vendor v;
    double celsius() const override { return (v.readF() - 32.0) * 5.0 / 9.0; }
};

double safeCap(double c) { return std::min(c, 100.0); }   // policy, in one place

int main() {
    FatAdapter fat;
    std::cout << "fat adapter  -> logged temp: " << fat.celsius()
              << " C  (true reading lost)\n";

    ThinAdapter thin;
    std::cout << "thin adapter -> logged temp: " << thin.celsius()
              << " C ; alarm shows capped " << safeCap(thin.celsius()) << " C\n";
    return 0;
}
