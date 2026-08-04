#include <functional>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

// ANTI-PATTERN: modelling actions as bare std::function<void()> when the feature
// needs UNDO. The history can store and replay them, but a callable cannot reverse
// itself, so there is nothing to call to undo -- you would have to maintain a
// parallel stack of hand-written inverse lambdas and keep the two perfectly in sync.
class HistoryBad {
public:
    void run(std::function<void()> action) {
        action();
        actions_.push_back(std::move(action));
    }
    // No undo(): a std::function<void()> carries no inverse to invoke.
private:
    std::vector<std::function<void()>> actions_;
};

// FIX: a command interface with execute() AND undo(). Each command carries its own
// reversal, so the history undoes generically -- no parallel bookkeeping.
struct Command {
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual ~Command() = default;
};

class Counter {
public:
    void add(int n) { value_ += n; }
    int value() const { return value_; }
private:
    int value_ = 0;
};

class AddCommand : public Command {
public:
    AddCommand(Counter& c, int n) : counter_(c), n_(n) {}
    void execute() override { counter_.add(n_); }
    void undo() override { counter_.add(-n_); }
private:
    Counter& counter_;
    int n_;
};

class HistoryGood {
public:
    void run(std::unique_ptr<Command> c) {
        c->execute();
        done_.push_back(std::move(c));
    }
    void undo() {
        if (done_.empty()) return;
        done_.back()->undo();
        done_.pop_back();
    }
private:
    std::vector<std::unique_ptr<Command>> done_;
};

int main() {
    std::cout << "== std::function: runs, but cannot undo ==\n";
    Counter c1;
    HistoryBad bad;
    bad.run([&c1] { c1.add(5); });
    std::cout << "counter: " << c1.value() << " (no undo available)\n";

    std::cout << "== command object: execute and undo ==\n";
    Counter c2;
    HistoryGood good;
    good.run(std::make_unique<AddCommand>(c2, 5));
    std::cout << "counter: " << c2.value() << "\n";      // 5
    good.undo();
    std::cout << "after undo: " << c2.value() << "\n";   // 0
    return 0;
}
