// Undo AND redo with two stacks of mementos.
//
// The caretaker keeps two stacks: undo holds states you can go back to, redo
// holds states you can go forward to. Before each edit it snapshots the current
// state onto the undo stack (and clears redo, since a fresh edit invalidates the
// forward history). Undo saves the current state onto redo, then restores the
// top of undo; redo does the mirror. This is the state-snapshot cousin of the
// Command chapter's undo, which instead stored reversible commands.

#include <iostream>
#include <string>
#include <vector>
#include <utility>

class Editor { // originator
    std::string text_;
public:
    class Memento {
        std::string text_;
        explicit Memento(std::string t) : text_(std::move(t)) {}
        friend class Editor;
    };
    void type(const std::string& s) { text_ += s; }
    const std::string& text() const { return text_; }
    Memento save() const { return Memento(text_); }
    void restore(const Memento& m) { text_ = m.text_; }
};

class History { // caretaker with undo + redo stacks
    Editor& editor_;
    std::vector<Editor::Memento> undo_;
    std::vector<Editor::Memento> redo_;
public:
    explicit History(Editor& e) : editor_(e) {}

    void backup() { // call before an edit
        undo_.push_back(editor_.save());
        redo_.clear(); // a new edit branch discards the redo history
    }
    void undo() {
        if (undo_.empty()) return;
        redo_.push_back(editor_.save());   // remember where we were, to redo
        editor_.restore(undo_.back());
        undo_.pop_back();
    }
    void redo() {
        if (redo_.empty()) return;
        undo_.push_back(editor_.save());
        editor_.restore(redo_.back());
        redo_.pop_back();
    }
};

int main() {
    Editor ed;
    History h(ed);

    h.backup(); ed.type("a");
    h.backup(); ed.type("b");
    h.backup(); ed.type("c");
    std::cout << "typed:  " << ed.text() << "\n"; // abc

    h.undo(); std::cout << "undo:   " << ed.text() << "\n"; // ab
    h.undo(); std::cout << "undo:   " << ed.text() << "\n"; // a
    h.redo(); std::cout << "redo:   " << ed.text() << "\n"; // ab
    return 0;
}
