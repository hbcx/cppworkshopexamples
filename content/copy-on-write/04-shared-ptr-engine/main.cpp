#include <iostream>
#include <memory>
#include <string>
#include <utility>

// Copy-on-write built on std::shared_ptr: its reference count IS our count,
// use_count() tells us whether the buffer is shared, and holding it as
// shared_ptr<const Data> makes the shared state read-only, so a write MUST go
// through detach().
class CowText {
    struct Data { std::string value; };
    std::shared_ptr<const Data> data_;

    Data& detach() {
        if (data_.use_count() > 1) {
            data_ = std::make_shared<Data>(*data_);   // private copy of the buffer
        }
        // We now own the only reference, and the underlying Data was created
        // non-const by make_shared, so modifying it here is well-defined.
        return const_cast<Data&>(*data_);
    }
public:
    explicit CowText(std::string s)
        : data_(std::make_shared<Data>(Data{std::move(s)})) {}

    const std::string& read()  const { return data_->value; }
    long        use_count()    const { return data_.use_count(); }
    const void* buffer()       const { return data_.get(); }

    void write(std::string s) { detach().value = std::move(s); }
};

int main() {
    CowText a{"alpha"};
    CowText b = a;   // shared_ptr copy: use_count 2, no deep copy
    std::cout << "use_count = " << a.use_count()
              << ", same buffer? " << (a.buffer() == b.buffer()) << '\n';   // 2, 1

    b.write("beta");   // detach: use_count > 1 -> private copy
    std::cout << "a=" << a.read() << " b=" << b.read()
              << " use_count(a)=" << a.use_count() << '\n';                 // alpha beta 1
    return 0;
}
