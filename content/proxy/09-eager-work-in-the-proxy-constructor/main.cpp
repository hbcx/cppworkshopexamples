#include <iostream>
#include <memory>
#include <string>
#include <utility>

struct Report {
    virtual void show() = 0;
    virtual ~Report() = default;
};

class RealReport : public Report {
public:
    explicit RealReport(const std::string& src) {
        std::cout << "building report from " << src << " (expensive)\n";
        data_ = "report(" + src + ")";
    }
    void show() override { std::cout << "show " << data_ << "\n"; }
private:
    std::string data_;
};

// ANTI-PATTERN: a virtual proxy that does the expensive work in its OWN
// constructor -- it eagerly builds the RealReport to "be ready". So merely
// creating the proxy already pays the full cost, and the laziness the proxy
// exists to provide is gone. It is an eager object with an extra layer.
class EagerProxy : public Report {
public:
    explicit EagerProxy(const std::string& src)
        : real_(std::make_unique<RealReport>(src)) {}    // builds NOW, not on use
    void show() override { real_->show(); }
private:
    std::unique_ptr<RealReport> real_;
};

// FIX: keep the proxy constructor cheap -- store only what is needed to build
// later, and build on first use.
class LazyProxy : public Report {
public:
    explicit LazyProxy(std::string src) : src_(std::move(src)) {}
    void show() override {
        if (!real_) real_ = std::make_unique<RealReport>(src_);
        real_->show();
    }
private:
    std::string src_;
    std::unique_ptr<RealReport> real_;
};

int main() {
    std::cout << "== eager proxy: cost paid at construction ==\n";
    EagerProxy eager("db");           // builds immediately
    std::cout << "(proxy constructed)\n";
    eager.show();

    std::cout << "== lazy proxy: cost deferred to first use ==\n";
    LazyProxy lazy("db");             // builds nothing yet
    std::cout << "(proxy constructed)\n";
    lazy.show();                      // builds here
    return 0;
}
