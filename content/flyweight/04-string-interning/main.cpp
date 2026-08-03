#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

// String interning is Flyweight applied to strings: equal text is stored once
// and shared. A SymbolTable hands back a shared handle to the single stored copy,
// so two symbols built from equal text ARE the same object -- and comparing them
// becomes a pointer comparison, O(1), instead of a character-by-character scan.
class SymbolTable {
public:
    std::shared_ptr<const std::string> intern(const std::string& text) {
        auto it = table_.find(text);
        if (it != table_.end()) return it->second;      // share the stored copy
        auto s = std::make_shared<const std::string>(text);
        table_.emplace(text, s);
        return s;
    }
    std::size_t distinct() const { return table_.size(); }
private:
    std::unordered_map<std::string, std::shared_ptr<const std::string>> table_;
};

int main() {
    SymbolTable symbols;
    // Imagine these identifiers coming from parsing source -- the same names recur.
    auto width1  = symbols.intern("width");
    auto height  = symbols.intern("height");
    auto width2  = symbols.intern("width");     // recurs -> shares width1's storage

    // Equal identifiers are now the SAME object: equality is a pointer compare.
    std::cout << "width #1 and #2 share storage: " << (width1 == width2 ? "yes" : "no") << "\n";
    std::cout << "width and height share storage: " << (width1 == height ? "yes" : "no") << "\n";
    std::cout << "distinct interned strings: " << symbols.distinct() << "\n";   // 2
    return 0;
}
