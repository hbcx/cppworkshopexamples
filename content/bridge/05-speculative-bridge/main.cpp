#include <iostream>
#include <memory>
#include <string>
#include <utility>

// ANTI-PATTERN: a Bridge introduced with only ONE implementation and no prospect
// of a second. The implementor interface, the single concrete implementor, and
// the abstraction-holds-a-pointer indirection all exist to allow a variation that
// will never happen -- speculative generality that costs an interface, a virtual
// call, and a heap allocation for nothing.
struct Storage {                        // implementor interface -- with one impl, forever
    virtual void save(const std::string& s) = 0;
    virtual ~Storage() = default;
};

class FileStorage : public Storage {    // the only implementor there will ever be
public:
    void save(const std::string& s) override { std::cout << "saved: " << s << "\n"; }
};

class DocumentBad {
public:
    explicit DocumentBad(std::unique_ptr<Storage> s) : storage_(std::move(s)) {}
    void write(const std::string& s) { storage_->save(s); }
private:
    std::unique_ptr<Storage> storage_;
};

// FIX: with a single implementation and no second dimension, it is just a class.
// No interface, no indirection, no allocation. If a real second backend ever
// appears, introduce the Bridge THEN -- when the variation is concrete.
class DocumentGood {
public:
    void write(const std::string& s) { std::cout << "saved: " << s << "\n"; }
};

int main() {
    DocumentBad bad(std::make_unique<FileStorage>());
    bad.write("bridge with one impl");

    DocumentGood good;
    good.write("plain class");
    return 0;
}
