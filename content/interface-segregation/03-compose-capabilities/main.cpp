// Compose a type from small interfaces, and hand each client one role.
//
// KeyValueStore inherits Reader (get) and Writer (put), so it is a full
// read-write store. But lookup() takes a const Reader& and cannot write, while
// seed() takes a Writer& and cannot read -- one object, two narrow views, each
// enforced by the compiler. This is the read-only view the Liskov chapter
// wanted, with no throwing stub.

#include <iostream>
#include <string>
#include <unordered_map>

// Two capability interfaces.
class Reader {
public:
    virtual ~Reader() {}
    virtual std::string get(const std::string& key) const = 0;
};

class Writer {
public:
    virtual ~Writer() {}
    virtual void put(const std::string& key, const std::string& value) = 0;
};

// A concrete type that fills both roles.
class KeyValueStore : public Reader, public Writer {
public:
    std::string get(const std::string& key) const override {
        std::unordered_map<std::string, std::string>::const_iterator it = data_.find(key);
        return it == data_.end() ? "" : it->second;
    }
    void put(const std::string& key, const std::string& value) override {
        data_[key] = value;
    }
private:
    std::unordered_map<std::string, std::string> data_;
};

// A writer client: sees only the Writer role.
void seed(Writer& w) {
    w.put("lang", "C++");
    w.put("year", "2011");
}

// A read-only client: sees only the Reader role, cannot mutate the store.
void report(const Reader& r) {
    std::cout << "lang=" << r.get("lang") << " year=" << r.get("year") << "\n";
}

int main() {
    KeyValueStore store;
    seed(store);     // passed as a Writer
    report(store);   // passed as a read-only Reader
}
