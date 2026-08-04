// Anti-pattern: inheritance for variation that should be composition.
//
// Template Method binds the varying step through the SUBCLASS, so the choice is
// fixed when the type is: you cannot change it on a live object, and when two
// steps vary independently you need one subclass per combination -- the cross
// product. Here a report varies by format AND by delivery; inheritance forces
// four subclasses for 2x2, and none can be reconfigured at runtime. The fix is
// Strategy: inject the varying steps as objects, so they compose (2+2, not 2x2)
// and can be swapped on one instance.

#include <iostream>
#include <functional>

// ---------- BAD: one subclass per (format, delivery) combination ----------
class ReportBad {
public:
    virtual ~ReportBad() = default;
    void publish() { std::cout << format() << " via " << deliver() << "\n"; }
protected:
    virtual const char* format() = 0;
    virtual const char* deliver() = 0;
};
// 2 formats x 2 deliveries = 4 classes, and a fifth of either axis doubles them.
class CsvEmail : public ReportBad {
    const char* format()  override { return "csv"; }
    const char* deliver() override { return "email"; }
};
class CsvFtp : public ReportBad {
    const char* format()  override { return "csv"; }
    const char* deliver() override { return "ftp"; }
};
class HtmlEmail : public ReportBad {
    const char* format()  override { return "html"; }
    const char* deliver() override { return "email"; }
};
class HtmlFtp : public ReportBad {
    const char* format()  override { return "html"; }
    const char* deliver() override { return "ftp"; }
};

// ---------- GOOD: inject the two varying steps; they compose ----------
class ReportGood {
    std::function<const char*()> format_;
    std::function<const char*()> deliver_;
public:
    ReportGood(std::function<const char*()> f, std::function<const char*()> d)
        : format_(std::move(f)), deliver_(std::move(d)) {}
    void publish() { std::cout << format_() << " via " << deliver_() << "\n"; }
    void setDelivery(std::function<const char*()> d) { deliver_ = std::move(d); }
};

int main() {
    std::cout << "--- bad (a class per combination) ---\n";
    CsvEmail{}.publish();
    HtmlFtp{}.publish();

    std::cout << "--- good (composed, and swappable at runtime) ---\n";
    auto csv  = [] { return "csv"; };
    auto html = [] { return "html"; };
    auto email = [] { return "email"; };
    auto ftp   = [] { return "ftp"; };
    ReportGood r(csv, email);
    r.publish();               // csv via email
    r.setDelivery(ftp);        // reconfigure the SAME object at runtime
    r.publish();               // csv via ftp
    ReportGood(html, ftp).publish(); // any pairing, no new class
    return 0;
}
