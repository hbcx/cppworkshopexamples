#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <utility>

class Texture {
public:
    explicit Texture(std::string name) : name_(std::move(name)) { std::cout << "load " << name_ << '\n'; }
    ~Texture() { std::cout << "free " << name_ << '\n'; }
    const std::string& name() const { return name_; }
private:
    std::string name_;
};

// A texture cache that does NOT own what it caches. It keeps a weak_ptr per key;
// get() returns the existing object if it is still alive, otherwise loads a fresh
// one. When the last real owner drops a texture, the cache's weak entry simply
// expires -- the cache never keeps anything alive on its own.
class TextureCache {
public:
    std::shared_ptr<Texture> get(const std::string& name) {
        std::weak_ptr<Texture>& slot = cache_[name];
        if (std::shared_ptr<Texture> hit = slot.lock()) {
            std::cout << "cache hit: " << name << '\n';
            return hit;                          // reuse the live instance
        }
        std::shared_ptr<Texture> fresh = std::make_shared<Texture>(name);
        slot = fresh;                            // remember it weakly
        return fresh;
    }
private:
    std::map<std::string, std::weak_ptr<Texture>> cache_;
};

int main() {
    TextureCache cache;

    auto a = cache.get("grass");                 // miss -> loads
    auto b = cache.get("grass");                 // hit -> same object as a
    std::cout << "a and b are the same object? " << (a == b ? "yes" : "no") << '\n';   // yes
    std::cout << "grass use_count = " << a.use_count() << '\n';                         // 2

    a.reset();
    b.reset();                                    // last owners gone -> grass freed
    std::cout << "-- both owners dropped --\n";

    auto c = cache.get("grass");                  // weak entry expired -> reload
    std::cout << "reloaded grass use_count = " << c.use_count() << '\n';                // 1
    return 0;
}
