#include <iostream>
#include <memory>
#include <string>
#include <utility>

class Texture {
public:
    explicit Texture(std::string name) : name_(std::move(name)) {
        std::cout << "load " << name_ << '\n';
    }
    ~Texture() { std::cout << "free " << name_ << '\n'; }
    const std::string& name() const { return name_; }
private:
    std::string name_;
};

int main() {
    std::weak_ptr<Texture> cache;   // observes, does not own

    {
        auto tex = std::make_shared<Texture>("grass");
        cache = tex;                // weak assignment: does NOT bump the owning count
        std::cout << "owners while alive = " << tex.use_count() << '\n';   // 1

        // lock() promotes the weak_ptr to a shared_ptr IF the object still lives.
        if (std::shared_ptr<Texture> hit = cache.lock()) {
            std::cout << "cache hit: " << hit->name() << '\n';
        }
    }   // tex is gone -> the Texture is freed, even though `cache` still refers to it

    std::cout << "expired? " << (cache.expired() ? "yes" : "no") << '\n';   // yes
    if (std::shared_ptr<Texture> hit = cache.lock()) {
        std::cout << "cache hit: " << hit->name() << '\n';
    } else {
        std::cout << "cache miss: reload needed\n";
    }
    return 0;
}
