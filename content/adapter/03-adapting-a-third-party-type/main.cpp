#include <iostream>

// --- third-party vendor header: we cannot modify this ---
struct VendorThermometer {
    double readFahrenheit() const { return 212.0; }   // boiling water
};

// --- our domain: everything speaks Celsius through this interface ---
struct Thermometer {
    virtual double celsius() const = 0;
    virtual ~Thermometer() = default;
};

// Adapter: wrap the vendor type and translate units on the way out. The vendor
// class stays untouched; our code depends only on Thermometer.
class VendorThermometerAdapter : public Thermometer {
public:
    explicit VendorThermometerAdapter(const VendorThermometer& vendor)
        : vendor_(vendor) {}
    double celsius() const override {
        return (vendor_.readFahrenheit() - 32.0) * 5.0 / 9.0;
    }
private:
    const VendorThermometer& vendor_;
};

// Reporting code depends only on the domain interface -- no Fahrenheit, no
// vendor type in sight.
void report(const Thermometer& t) {
    std::cout << "temperature: " << t.celsius() << " C\n";
}

int main() {
    VendorThermometer vendor;
    VendorThermometerAdapter adapter(vendor);
    report(adapter);   // vendor's Fahrenheit, consumed as Celsius
    return 0;
}
