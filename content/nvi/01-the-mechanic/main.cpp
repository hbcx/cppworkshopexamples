#include <iostream>

// The Non-Virtual Interface (NVI) idiom: the public method is non-virtual and
// owns the fixed frame; the one varying step is a PRIVATE virtual it calls.
class Report {
public:
    virtual ~Report() = default;

    // Public, non-virtual: the single entry point clients call. The base owns
    // the fixed frame -- header and footer -- around the one varying part.
    void render() const {
        std::cout << "=== BEGIN ===\n";
        body();                       // the ONE customization point
        std::cout << "=== END ===\n";
    }

private:
    // The customization point: private virtual. Subclasses override it; only
    // render() ever calls it, so it can never run without the frame.
    virtual void body() const = 0;
};

class SalesReport : public Report {
    void body() const override { std::cout << "sales: 1200 units\n"; }
};
class ErrorReport : public Report {
    void body() const override { std::cout << "errors: 3 warnings\n"; }
};

int main() {
    SalesReport{}.render();
    ErrorReport{}.render();
    return 0;
}
