#include <iostream>
#include <map>
#include <memory>
#include <string>

// ANTI-PATTERN: the factory holds a shared_ptr to every flyweight it ever built,
// so nothing is released. In a long-running process that interns unbounded
// distinct keys -- request ids, user-generated tags, session names -- the pool
// grows without bound: an interning cache that is really a memory leak.
class StrongCache {
public:
    std::shared_ptr<std::string> get(const std::string& key) {
        auto& s = pool_[key];
        if (!s) s = std::make_shared<std::string>(key);
        return s;
    }
    std::size_t size() const { return pool_.size(); }
private:
    std::map<std::string, std::shared_ptr<std::string>> pool_;   // strong -> never frees
};

// FIX: the factory holds WEAK pointers. A flyweight lives only while some client
// holds it; once the last user drops it the weak entry expires, so the pool
// tracks only what is actually in use and cannot grow without bound.
class WeakCache {
public:
    std::shared_ptr<std::string> get(const std::string& key) {
        auto& weak = pool_[key];
        if (auto s = weak.lock()) return s;          // still alive -> share it
        auto s = std::make_shared<std::string>(key);
        weak = s;
        return s;
    }
    std::size_t liveCount() const {
        std::size_t n = 0;
        for (const auto& kv : pool_) {
            if (!kv.second.expired()) ++n;
        }
        return n;
    }
private:
    std::map<std::string, std::weak_ptr<std::string>> pool_;
};

int main() {
    std::cout << "== strong cache: entries never released ==\n";
    StrongCache strong;
    {
        auto a = strong.get("temp-1");
        auto b = strong.get("temp-2");
    }   // both users drop here...
    std::cout << "pool size after users gone: " << strong.size() << "\n";   // 2 -- leaked

    std::cout << "== weak cache: entries expire with their last user ==\n";
    WeakCache weak;
    {
        auto a = weak.get("temp-1");
        auto b = weak.get("temp-2");
    }   // both users drop here...
    std::cout << "live entries after users gone: " << weak.liveCount() << "\n";   // 0
    return 0;
}
