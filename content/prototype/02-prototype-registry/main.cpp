#include <iostream>
#include <map>
#include <memory>
#include <string>

struct Enemy {
    virtual std::unique_ptr<Enemy> clone() const = 0;
    virtual std::string describe() const = 0;
    virtual ~Enemy() = default;
};

struct Grunt : Enemy {
    int hp;
    std::string weapon;
    Grunt(int hp_, std::string weapon_) : hp(hp_), weapon(std::move(weapon_)) {}
    std::unique_ptr<Enemy> clone() const override { return std::make_unique<Grunt>(*this); }
    std::string describe() const override { return "Grunt(hp=" + std::to_string(hp) + ",weapon=" + weapon + ")"; }
};

struct Boss : Enemy {
    int hp;
    int phases;
    Boss(int hp_, int phases_) : hp(hp_), phases(phases_) {}
    std::unique_ptr<Enemy> clone() const override { return std::make_unique<Boss>(*this); }
    std::string describe() const override { return "Boss(hp=" + std::to_string(hp) + ",phases=" + std::to_string(phases) + ")"; }
};

// The registry: one configured prototype per name, cloned on create().
class EnemyRegistry {
public:
    void registerPrototype(std::string name, std::unique_ptr<Enemy> proto) {
        prototypes_[std::move(name)] = std::move(proto);
    }
    std::unique_ptr<Enemy> create(const std::string& name) const {
        auto it = prototypes_.find(name);
        return it == prototypes_.end() ? nullptr : it->second->clone();
    }
private:
    std::map<std::string, std::unique_ptr<Enemy>> prototypes_;
};

int main() {
    // Configure each template once, at registration time.
    EnemyRegistry registry;
    registry.registerPrototype("grunt", std::make_unique<Grunt>(30, "pistol"));
    registry.registerPrototype("boss", std::make_unique<Boss>(500, 3));

    // Spawn instances by name -- each is an independent clone of its template.
    for (const char* name : {"grunt", "grunt", "boss"}) {
        std::unique_ptr<Enemy> e = registry.create(name);
        std::cout << "spawned " << name << ": " << e->describe() << '\n';
    }

    // An unknown key clones nothing.
    std::cout << "unknown key -> " << (registry.create("dragon") ? "made one" : "null") << '\n';
    return 0;
}
