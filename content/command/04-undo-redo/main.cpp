#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

// Command with undo: it stores enough to REVERSE its own effect. A history of
// executed commands drives undo; a second stack of undone ones drives redo.
struct Command {
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual ~Command() = default;
};

// Receiver.
class Editor {
public:
    void append(const std::string& s) { text_ += s; }
    void chop(std::size_t n) { text_.erase(text_.size() - n); }
    const std::string& text() const { return text_; }
private:
    std::string text_;
};

class AppendCommand : public Command {
public:
    AppendCommand(Editor& e, std::string s) : editor_(e), s_(std::move(s)) {}
    void execute() override { editor_.append(s_); }
    void undo() override { editor_.chop(s_.size()); }   // the exact reverse of execute()
private:
    Editor& editor_;
    std::string s_;
};

// Invoker with history: runs commands, and can undo and redo them.
class History {
public:
    void run(std::unique_ptr<Command> c) {
        c->execute();
        done_.push_back(std::move(c));
        undone_.clear();               // a fresh action invalidates the redo stack
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
    Editor editor;
    History history;

    history.run(std::make_unique<AppendCommand>(editor, "Hello"));
    history.run(std::make_unique<AppendCommand>(editor, ", world"));
    std::cout << "two appends: \"" << editor.text() << "\"\n";   // Hello, world

    history.undo();
    std::cout << "after undo:  \"" << editor.text() << "\"\n";   // Hello

    history.redo();
    std::cout << "after redo:  \"" << editor.text() << "\"\n";   // Hello, world

    history.undo();
    history.undo();
    std::cout << "two undos:   \"" << editor.text() << "\"\n";   // (empty)
    return 0;
}
