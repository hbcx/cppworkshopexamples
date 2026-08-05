#ifndef PIMPL_FIREWALL_REPORT_HPP
#define PIMPL_FIREWALL_REPORT_HPP

#include <cstddef>
#include <memory>
#include <string>

// No <vector>, no <sstream>: clients that include this header do not pull those
// in, and do not recompile when Report's internals change.
class Report {
public:
    Report();
    ~Report();
    void addRow(const std::string& label, int value);
    std::string render() const;
    static std::size_t footprint(); // sizeof the public object

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

#endif
