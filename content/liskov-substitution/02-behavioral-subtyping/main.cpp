// LSP is behavioural: a subtype must honour the base's behaviour, not just its
// signatures.
//
// KeyValueStore's contract is more than its method list: after put(k, v),
// get(k) returns v. InMemoryStore keeps that promise, so roundTrips() -- code
// written to the contract -- works. A store that merely compiles but forgets or
// rewrites values would break every such caller, and the compiler would not
// notice.

#include <iostream>
#include <string>
#include <unordered_map>

class KeyValueStore {
public:
    virtual ~KeyValueStore() {}
    // Contract: after put(k, v), a later get(k) returns exactly v.
    virtual void put(const std::string& key, const std::string& value) = 0;
    virtual std::string get(const std::string& key) const = 0;
};

class InMemoryStore : public KeyValueStore {
public:
    void put(const std::string& key, const std::string& value) override {
        data_[key] = value;
    }
    std::string get(const std::string& key) const override {
        std::unordered_map<std::string, std::string>::const_iterator it = data_.find(key);
        return it == data_.end() ? "" : it->second;
    }
private:
    std::unordered_map<std::string, std::string> data_;
};

// Written to the CONTRACT, not to a concrete store: the value read back must
// equal the value written.
bool roundTrips(KeyValueStore& store, const std::string& key, const std::string& value) {
    store.put(key, value);
    return store.get(key) == value;
}

int main() {
    InMemoryStore store;
    std::cout << "round-trip 'name'->'Ada': "
              << (roundTrips(store, "name", "Ada") ? "ok" : "BROKEN") << "\n";
    std::cout << "round-trip 'city'->'London': "
              << (roundTrips(store, "city", "London") ? "ok" : "BROKEN") << "\n";
    std::cout << "stored name = " << store.get("name") << "\n";
    // A subtype that only *looks* like a store -- forgetting or rewriting the
    // value -- would compile the same and make roundTrips() return BROKEN.
}
