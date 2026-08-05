#include "report.hpp"

// The heavy includes live HERE, not in the header.
#include <sstream>
#include <utility>
#include <vector>

struct Report::Impl {
    std::vector<std::pair<std::string, int>> rows; // internals invisible to clients
};

Report::Report() : impl_(std::make_unique<Impl>()) {}
Report::~Report() = default;

void Report::addRow(const std::string& label, int value) {
    impl_->rows.push_back(std::make_pair(label, value));
}

std::string Report::render() const {
    std::ostringstream os;
    for (const std::pair<std::string, int>& row : impl_->rows)
        os << row.first << "=" << row.second << " ";
    return os.str();
}

std::size_t Report::footprint() { return sizeof(Report); }
