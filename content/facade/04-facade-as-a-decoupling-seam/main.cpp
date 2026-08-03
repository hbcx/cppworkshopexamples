#include <iostream>
#include <memory>
#include <string>

// The facade's interface: what the client depends on. One method, and -- this is
// the point -- no subsystem types in its signature.
struct ReportFacade {
    virtual std::string build(const std::string& title) const = 0;
    virtual ~ReportFacade() = default;
};

// A subsystem: several classes with their own APIs, none of which the client
// will ever name.
class DataSource {
public:
    std::string load() const { return "rows=3"; }
};

class Formatter {
public:
    std::string format(const std::string& title, const std::string& data) const {
        return title + " {" + data + "}";
    }
};

// Concrete facade: orchestrates the real subsystem behind the interface.
class RealReportFacade : public ReportFacade {
public:
    std::string build(const std::string& title) const override {
        return formatter_.format(title, source_.load());
    }
private:
    DataSource source_;
    Formatter formatter_;
};

// Because the client depends only on the facade INTERFACE, the entire subsystem
// can be replaced without touching the client -- here by a fake that involves no
// DataSource and no Formatter, so the client can be tested in isolation.
class FakeReportFacade : public ReportFacade {
public:
    std::string build(const std::string& title) const override {
        return title + " {stub}";
    }
};

// Client: depends on the interface, knows nothing of the subsystem behind it.
void printReport(const ReportFacade& facade, const std::string& title) {
    std::cout << facade.build(title) << '\n';
}

int main() {
    std::unique_ptr<ReportFacade> real = std::make_unique<RealReportFacade>();
    printReport(*real, "Sales");        // Sales {rows=3}

    // Same client, whole subsystem swapped out behind the same interface.
    std::unique_ptr<ReportFacade> fake = std::make_unique<FakeReportFacade>();
    printReport(*fake, "Sales");        // Sales {stub}
    return 0;
}
