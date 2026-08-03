#include <iostream>
#include <memory>
#include <string>
#include <utility>

// ANTI-PATTERN: one class accretes every optional feature as a constructor flag,
// and render() grows an if per flag. Each new option edits this class and its
// method (breaking open/closed), the flags can interact in ways no one tested,
// and a caller cannot add a feature the author did not foresee.
class ReportBad {
public:
    ReportBad(std::string body, bool header, bool footer, bool border)
        : body_(std::move(body)), header_(header), footer_(footer), border_(border) {}
    std::string render() const {
        std::string out;
        if (header_) out += "== REPORT ==\n";
        if (border_) out += "| ";
        out += body_;
        if (border_) out += " |";
        out += "\n";
        if (footer_) out += "-- end --\n";
        return out;
    }
private:
    std::string body_;
    bool header_;
    bool footer_;
    bool border_;
};

// FIX: each option is a decorator over a Component, added only when wanted and in
// any order. A new option is a new class -- the existing ones never change.
struct Report {
    virtual std::string render() const = 0;
    virtual ~Report() = default;
};

class Body : public Report {
public:
    explicit Body(std::string body) : body_(std::move(body)) {}
    std::string render() const override { return body_ + "\n"; }
private:
    std::string body_;
};

class ReportDecorator : public Report {
public:
    explicit ReportDecorator(std::unique_ptr<Report> inner) : inner_(std::move(inner)) {}
protected:
    std::string inner() const { return inner_->render(); }
private:
    std::unique_ptr<Report> inner_;
};

class Header : public ReportDecorator {
public:
    using ReportDecorator::ReportDecorator;
    std::string render() const override { return "== REPORT ==\n" + inner(); }
};

class Footer : public ReportDecorator {
public:
    using ReportDecorator::ReportDecorator;
    std::string render() const override { return inner() + "-- end --\n"; }
};

int main() {
    ReportBad bad("sales up 4%", true, true, false);
    std::cout << bad.render();

    std::cout << "----\n";

    // Same output, but composed -- and a new feature would be a new class, not an
    // edit to an existing one, and not another bool threaded through the ctor.
    std::unique_ptr<Report> good =
        std::make_unique<Footer>(
            std::make_unique<Header>(
                std::make_unique<Body>("sales up 4%")));
    std::cout << good->render();
    return 0;
}
