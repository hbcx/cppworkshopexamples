#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

struct Command {
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual ~Command() = default;
};

class Doc {
public:
    void append(char c) { text_ += c; }
    void chop() { if (!text_.empty()) text_.pop_back(); }
    std::string text() const { return text_; }
private:
    std::string text_;
};

class Type : public Command {
public:
    Type(Doc& d, char c) : doc_(d), c_(c) {}
    void execute() override { doc_.append(c_); }
    void undo() override { doc_.chop(); }
private:
    Doc& doc_;
    char c_;
};

// ANTI-PATTERN: run() does NOT clear the redo stack. After you undo an action and
// then do something new, the old undone command still sits on the redo stack, so a
// later redo re-applies a command that belonged to a future you abandoned --
// corrupting the state with a stale edit.
class HistoryBad {
public:
    void run(std::unique_ptr<Command> c) { c->execute(); done_.push_back(std::move(c)); }
    void undo() {
        if (done_.empty()) return;
        done_.back()->undo();
        undone_.push_back(std::move(done_.back()));
        done_.pop_back();
    }
    void redo() {
        if (undone_.empty()) return;
        undone_.back()->execute();
        done_.push_back(std::move(undone_.back()));
        undone_.pop_back();
    }
private:
    std::vector<std::unique_ptr<Command>> done_;
    std::vector<std::unique_ptr<Command>> undone_;
};

// FIX: a new action clears the redo stack -- the undone future no longer exists.
class HistoryGood {
public:
    void run(std::unique_ptr<Command> c) {
        c->execute();
        done_.push_back(std::move(c));
        undone_.clear();                 // the one line that keeps redo honest
    }
    void undo() {
        if (done_.empty()) return;
        done_.back()->undo();
        undone_.push_back(std::move(done_.back()));
        done_.pop_back();
    }
    void redo() {
        if (undone_.empty()) return;
        undone_.back()->execute();
        done_.push_back(std::move(undone_.back()));
        undone_.pop_back();
    }
private:
    std::vector<std::unique_ptr<Command>> done_;
    std::vector<std::unique_ptr<Command>> undone_;
};

int main() {
    std::cout << "== redo stack not cleared ==\n";
    Doc d1;
    HistoryBad bad;
    bad.run(std::make_unique<Type>(d1, 'A'));    // "A"
    bad.undo();                                  // ""  (A is now on the redo stack)
    bad.run(std::make_unique<Type>(d1, 'B'));    // "B" (but A still lingers in redo)
    bad.redo();                                  // re-applies the stale A -> "BA"
    std::cout << "text: \"" << d1.text() << "\" (wanted \"B\")\n";

    std::cout << "== redo stack cleared on a new action ==\n";
    Doc d2;
    HistoryGood good;
    good.run(std::make_unique<Type>(d2, 'A'));
    good.undo();
    good.run(std::make_unique<Type>(d2, 'B'));   // clears the redo stack
    good.redo();                                 // nothing to redo -> no change
    std::cout << "text: \"" << d2.text() << "\" (wanted \"B\")\n";
    return 0;
}
