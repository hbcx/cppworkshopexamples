#include <forward_list>
#include <iostream>
#include <string>

// A tiny undo stack: every action is pushed on the front, undo pops the front.
// Only one end is ever touched, so the singly-linked list is a perfect fit --
// one pointer per node and no unused back pointer.
class UndoStack {
    std::forward_list<std::string> actions_;
public:
    void doAction(const std::string& a) { actions_.push_front(a); }  // O(1)
    bool empty() const { return actions_.empty(); }
    std::string undo() {                                             // O(1)
        std::string top = actions_.front();
        actions_.pop_front();
        return top;
    }
};

int main() {
    UndoStack u;
    u.doAction("type H");
    u.doAction("type i");
    u.doAction("bold");

    std::cout << "undo: " << u.undo() << '\n';   // bold  (last in, first out)
    std::cout << "undo: " << u.undo() << '\n';   // type i
    std::cout << "undo: " << u.undo() << '\n';   // type H
    std::cout << "empty now? " << std::boolalpha << u.empty() << '\n';
    return 0;
}
