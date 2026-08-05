// Single Responsibility Principle -- the mechanic.
//
// A class should have ONE reason to change. The bad Invoice below mixes two:
// computing the amount owed (a money rule) and turning itself into a printable
// block (a layout rule). Those change for different reasons and by different
// people, so the class has two reasons to change -- one too many.
//
// The good version splits them: Invoice holds the data and the one rule that is
// its own (the total); InvoiceFormatter owns how an invoice looks. Same output,
// but now a layout change touches only the formatter.

#include <iostream>
#include <string>
#include <utility>
#include <vector>

struct Line {
    std::string item;
    double price;
    int qty;
};

static std::string money(double v) {
    long cents = static_cast<long>(v * 100.0 + 0.5);
    std::string frac = std::to_string(cents % 100);
    if (frac.size() < 2) frac = "0" + frac;
    return std::to_string(cents / 100) + "." + frac;
}

namespace bad {

// Two responsibilities welded together: the money rule AND the text layout.
class Invoice {
public:
    explicit Invoice(std::string customer) : customer_(std::move(customer)) {}

    void addLine(const std::string& item, double price, int qty) {
        lines_.push_back(Line{item, price, qty});
    }

    double total() const {                       // money rule
        double sum = 0.0;
        for (const Line& l : lines_) sum += l.price * l.qty;
        return sum;
    }

    std::string asText() const {                 // layout rule -- a different reason to change
        std::string out = "Invoice for " + customer_ + "\n";
        for (const Line& l : lines_)
            out += "  " + l.item + " x" + std::to_string(l.qty) + "\n";
        out += "  TOTAL: " + money(total()) + "\n";
        return out;
    }

private:
    std::string customer_;
    std::vector<Line> lines_;
};

} // namespace bad

namespace good {

// ONE responsibility: hold the invoice and the money rule.
class Invoice {
public:
    explicit Invoice(std::string customer) : customer_(std::move(customer)) {}

    void addLine(const std::string& item, double price, int qty) {
        lines_.push_back(Line{item, price, qty});
    }

    const std::string& customer() const { return customer_; }
    const std::vector<Line>& lines() const { return lines_; }

    double total() const {
        double sum = 0.0;
        for (const Line& l : lines_) sum += l.price * l.qty;
        return sum;
    }

private:
    std::string customer_;
    std::vector<Line> lines_;
};

// ONE responsibility: how an invoice looks as text. The only class a layout
// change ever needs to touch.
class InvoiceFormatter {
public:
    std::string asText(const Invoice& inv) const {
        std::string out = "Invoice for " + inv.customer() + "\n";
        for (const Line& l : inv.lines())
            out += "  " + l.item + " x" + std::to_string(l.qty) + "\n";
        out += "  TOTAL: " + money(inv.total()) + "\n";
        return out;
    }
};

} // namespace good

int main() {
    bad::Invoice b("Acme Corp");
    b.addLine("Widget", 9.99, 3);
    b.addLine("Gadget", 19.95, 1);
    std::cout << "--- one class does both ---\n" << b.asText();

    good::Invoice inv("Acme Corp");
    inv.addLine("Widget", 9.99, 3);
    inv.addLine("Gadget", 19.95, 1);
    good::InvoiceFormatter formatter;
    std::cout << "--- data-plus-rule + formatter (same output) ---\n"
              << formatter.asText(inv);
    std::cout << "Amount owed stays a pure number: " << money(inv.total()) << "\n";
}
