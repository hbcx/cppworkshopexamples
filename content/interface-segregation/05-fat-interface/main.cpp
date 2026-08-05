// Anti-pattern: a fat interface.
//
// IMultiFunctionDevice bundles print, scan and fax. A SimplePrinter can only
// print, but to satisfy the interface it must implement scan and fax with
// throwing stubs. Code that calls scan through the base throws the moment it
// meets a plain printer -- it compiles, and fails at run time.
//
// The fix splits the interface into Printer, Scanner and Fax roles a class
// inherits only as far as it supports them.

#include <iostream>
#include <stdexcept>
#include <string>

// ---------- one interface asks for everything ----------
namespace bad {

class IMultiFunctionDevice {
public:
    virtual ~IMultiFunctionDevice() {}
    virtual std::string print(const std::string& doc) = 0;
    virtual std::string scan() = 0;
    virtual std::string fax(const std::string& doc) = 0;
};

class SimplePrinter : public IMultiFunctionDevice {
public:
    std::string print(const std::string& doc) override { return "printed: " + doc; }
    std::string scan() override { throw std::logic_error("no scanner"); } // forced stub
    std::string fax(const std::string&) override { throw std::logic_error("no fax"); }
};

void scanCover(IMultiFunctionDevice& d) {
    try {
        std::string page = d.scan(); // assumes any device can scan
        std::cout << "[bad] " << page << "\n";
    } catch (const std::exception& e) {
        std::cout << "[bad] ERROR " << e.what() << "\n";
    }
}

} // namespace bad

// ---------- one small interface per capability ----------
namespace good {

class Printer {
public:
    virtual ~Printer() {}
    virtual std::string print(const std::string& doc) = 0;
};

class Scanner {
public:
    virtual ~Scanner() {}
    virtual std::string scan() = 0;
};

class SimplePrinter : public Printer { // only the role it can fill
public:
    std::string print(const std::string& doc) override { return "printed: " + doc; }
};

class AllInOne : public Printer, public Scanner {
public:
    std::string print(const std::string& doc) override { return "printed: " + doc; }
    std::string scan() override { return "scanned page"; }
};

// Needs a scanner -- a plain printer cannot be passed here.
void scanCover(Scanner& s) { std::cout << "[good] " << s.scan() << "\n"; }

} // namespace good

int main() {
    bad::SimplePrinter bp;
    std::cout << "[bad] " << bp.print("memo") << "\n";
    bad::scanCover(bp); // throws inside, caught

    good::SimplePrinter gp;
    good::AllInOne aio;
    std::cout << "[good] " << gp.print("memo") << "\n";
    good::scanCover(aio); // only a real scanner reaches here
}
