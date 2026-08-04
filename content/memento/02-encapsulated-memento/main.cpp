// Encapsulation: the memento is opaque to the caretaker.
//
// The point of Memento is not just to save state, but to save it WITHOUT
// exposing it. Here the caretaker (History) stores a stack of the document's
// mementos and can hand them back, but it cannot read or modify what is inside
// one: the memento's state is private to Document, which alone is a friend.
// History coordinates undo without ever learning how a Document stores its text
// -- so the two can evolve independently. Try to read stack_.back().text_ from
// History and it will not compile.

#include <iostream>
#include <string>
#include <vector>
#include <utility>

class Document { // the originator
    std::string text_;
public:
    class Memento { // opaque: only Document can build or read it
        std::string text_;
        explicit Memento(std::string t) : text_(std::move(t)) {}
        friend class Document;
    };

    void write(const std::string& s) { text_ += s; }
    const std::string& text() const { return text_; }

    Memento snapshot() const { return Memento(text_); }
    void restore(const Memento& m) { text_ = m.text_; }
};

class History { // the caretaker: holds mementos, never inspects them
    std::vector<Document::Memento> stack_;
public:
    void backup(const Document& doc) { stack_.push_back(doc.snapshot()); }
    void undo(Document& doc) {
        if (stack_.empty()) return;
        doc.restore(stack_.back()); // hand the sealed box back to its owner
        stack_.pop_back();
    }
    // History cannot do: stack_.back().text_  -- text_ is private to Document.
    std::size_t depth() const { return stack_.size(); }
};

int main() {
    Document doc;
    History history;

    doc.write("chapter 1. ");
    history.backup(doc); // save "chapter 1. "
    doc.write("chapter 2. ");
    history.backup(doc); // save "chapter 1. chapter 2. "
    doc.write("oops");

    std::cout << "now:  " << doc.text() << "\n";
    history.undo(doc); // back to "chapter 1. chapter 2. "
    std::cout << "undo: " << doc.text() << "\n";
    history.undo(doc); // back to "chapter 1. "
    std::cout << "undo: " << doc.text() << "\n";
    return 0;
}
