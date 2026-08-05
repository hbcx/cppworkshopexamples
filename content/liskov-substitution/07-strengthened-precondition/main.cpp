// Anti-pattern: a subtype that strengthens a precondition.
//
// UserService accepts ANY id and never fails. CachedUserService demands an id
// in [1, 100] and throws otherwise -- it requires MORE of its callers than the
// base. Code written to the base passes id 0 (valid for the base) and breaks
// with the subclass.
//
// The fix: a subtype must accept everything the base accepts, falling back
// rather than rejecting.

#include <iostream>
#include <stdexcept>
#include <string>

// ---------- the subclass demands more than the base ----------
namespace bad {

class UserService {
public:
    virtual ~UserService() {}
    // precondition: none -- any id is accepted.
    virtual std::string lookup(int id) const {
        return "user#" + std::to_string(id);
    }
};

class CachedUserService : public UserService {
public:
    // STRENGTHENED precondition: id must be in [1, 100], else it throws.
    std::string lookup(int id) const override {
        if (id < 1 || id > 100) throw std::out_of_range("id must be 1..100");
        return "cached#" + std::to_string(id);
    }
};

// Written to the base contract: id 0 is fine.
void show(const UserService& s) {
    try {
        std::string r = s.lookup(0); // may throw before anything is printed
        std::cout << "[bad] " << r << "\n";
    } catch (const std::exception& e) {
        std::cout << "[bad] ERROR " << e.what() << "\n";
    }
}

} // namespace bad

// ---------- the subclass honours the base's input domain ----------
namespace good {

class UserService {
public:
    virtual ~UserService() {}
    virtual std::string lookup(int id) const {
        return "user#" + std::to_string(id);
    }
};

class CachedUserService : public UserService {
public:
    // Accepts every id the base accepts: in-range ids are served from cache,
    // out-of-range ids fall back to the base behaviour instead of throwing.
    std::string lookup(int id) const override {
        if (id < 1 || id > 100) return UserService::lookup(id);
        return "cached#" + std::to_string(id);
    }
};

void show(const UserService& s) {
    std::cout << "[good] " << s.lookup(0) << "\n";
}

} // namespace good

int main() {
    bad::UserService baseSvc;
    bad::CachedUserService cachedSvc;
    bad::show(baseSvc);    // user#0
    bad::show(cachedSvc);  // throws: substitution broken

    good::UserService gbase;
    good::CachedUserService gcached;
    good::show(gbase);     // user#0
    good::show(gcached);   // user#0 via fallback: substitutable
}
