#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <string>

// ANTI-PATTERN: an interning factory with no synchronisation. Two threads calling
// get("x") at once can BOTH find it missing and BOTH construct it, so the
// one-instance-per-key invariant breaks and a construction is lost -- and the
// std::map itself, written by two threads at once, is a data race and undefined
// behaviour. TSan would flag it; here the vulnerable interleaving is reproduced
// deterministically in a single thread (no real race, no UB) to show the effect.
class UnsyncedFactory {
public:
    bool has(const std::string& key) const { return pool_.count(key) != 0; }
    void create(const std::string& key) {
        ++builds_;
        pool_[key] = std::make_shared<std::string>(key);   // last writer wins
    }
    int builds() const { return builds_; }
private:
    std::map<std::string, std::shared_ptr<std::string>> pool_;
    int builds_ = 0;
};

// FIX: a mutex makes the check-and-create one atomic step, so only the first
// caller builds and every other shares -- correct no matter how threads interleave.
class SyncedFactory {
public:
    std::shared_ptr<std::string> get(const std::string& key) {
        std::lock_guard<std::mutex> lock(m_);
        auto it = pool_.find(key);
        if (it != pool_.end()) return it->second;
        ++builds_;
        auto s = std::make_shared<std::string>(key);
        pool_[key] = s;
        return s;
    }
    int builds() const { return builds_; }
private:
    std::mutex m_;
    std::map<std::string, std::shared_ptr<std::string>> pool_;
    int builds_ = 0;
};

int main() {
    const std::string key = "x";

    std::cout << "== unsynchronised: the check-then-create race, reproduced ==\n";
    UnsyncedFactory bad;
    // Thread A checks: missing. Thread B checks: also missing, because A has not
    // inserted yet. Both then create -- two objects built for one key.
    bool aMiss = !bad.has(key);
    bool bMiss = !bad.has(key);
    if (aMiss) bad.create(key);
    if (bMiss) bad.create(key);        // clobbers A's insert
    std::cout << "objects built for one key: " << bad.builds() << "\n";     // 2 -- wrong

    std::cout << "== synchronised: check-and-create under a lock ==\n";
    SyncedFactory good;
    good.get(key);
    good.get(key);
    std::cout << "objects built for one key: " << good.builds() << "\n";    // 1
    return 0;
}
