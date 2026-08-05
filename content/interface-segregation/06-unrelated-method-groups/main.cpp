// Anti-pattern: one interface bundling unrelated method groups.
//
// IEmployee carries payroll, scheduling and reporting methods. Each client uses
// only its own group, yet runPayroll -- which calls only calculatePay -- depends
// on the whole interface, so a change to reporting or scheduling recompiles and
// re-tests it for nothing.
//
// The fix splits by client into Payable, Schedulable and Reportable; a Developer
// fills all three, but each caller depends on just one.

#include <iostream>
#include <string>

// ---------- three clients' worth of methods in one interface ----------
namespace bad {

class IEmployee {
public:
    virtual ~IEmployee() {}
    virtual long calculatePay() const = 0;        // payroll client
    virtual void assignShift(int day) = 0;         // scheduling client
    virtual std::string statusReport() const = 0;  // reporting client
};

class Developer : public IEmployee {
public:
    long calculatePay() const override { return 8000; }
    void assignShift(int day) override { shift_ = day; }
    std::string statusReport() const override { return "developer, shift " + std::to_string(shift_); }
private:
    int shift_ = 0;
};

// Uses only calculatePay, but depends on all of IEmployee.
long runPayroll(const IEmployee& e) { return e.calculatePay(); }

} // namespace bad

// ---------- one interface per client ----------
namespace good {

class Payable {
public:
    virtual ~Payable() {}
    virtual long calculatePay() const = 0;
};

class Schedulable {
public:
    virtual ~Schedulable() {}
    virtual void assignShift(int day) = 0;
};

class Reportable {
public:
    virtual ~Reportable() {}
    virtual std::string statusReport() const = 0;
};

class Developer : public Payable, public Schedulable, public Reportable {
public:
    long calculatePay() const override { return 8000; }
    void assignShift(int day) override { shift_ = day; }
    std::string statusReport() const override { return "developer, shift " + std::to_string(shift_); }
private:
    int shift_ = 0;
};

// Depends only on Payable -- reporting or scheduling changes cannot touch it.
long runPayroll(const Payable& p) { return p.calculatePay(); }

} // namespace good

int main() {
    bad::Developer bd;
    std::cout << "[bad]  pay = " << bad::runPayroll(bd) << "\n";

    good::Developer gd;
    gd.assignShift(3);
    std::cout << "[good] pay = " << good::runPayroll(gd) << "\n";
    std::cout << "[good] " << gd.statusReport() << "\n";
}
