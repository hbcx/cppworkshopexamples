// Memento basics: save a snapshot, restore it later.
//
// Three roles. The ORIGINATOR (TextEditor) is the object whose state we want to
// snapshot and roll back. The MEMENTO is an opaque snapshot of that state. The
// CARETAKER (here, main) asks the originator for a memento, holds it, and later
// hands it back to restore -- without ever looking inside it. The editor types
// some text, we snapshot it, type more, then restore to the snapshot.

#include <iostream>
#include <string>
#include <utility>

class TextEditor { // the originator
    std::string content_;
public:
    // The memento: only TextEditor can build one or read its contents.
    class Memento {
        std::string content_;
        explicit Memento(std::string c) : content_(std::move(c)) {}
        friend class TextEditor;
    };

    void type(const std::string& text) { content_ += text; }
    const std::string& content() const { return content_; }

    Memento save() const { return Memento(content_); }        // produce a snapshot
    void restore(const Memento& m) { content_ = m.content_; } // roll back to it
};

int main() {
    TextEditor editor;
    editor.type("Hello");

    TextEditor::Memento checkpoint = editor.save(); // caretaker keeps the snapshot
    editor.type(", world");
    std::cout << "before restore: " << editor.content() << "\n";

    editor.restore(checkpoint); // roll back to "Hello"
    std::cout << "after restore:  " << editor.content() << "\n";
    return 0;
}
