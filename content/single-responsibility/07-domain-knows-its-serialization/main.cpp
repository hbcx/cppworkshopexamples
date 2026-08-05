// Anti-pattern: a domain object that serializes and persists itself.
//
// The bad Customer carries toJson, toXml and saveToDb. The JSON shape, the
// database schema and the business rules now all live in one class, so an API
// version bump, a column rename, and a rule change all reopen it -- and every
// entity that copied the pattern. A single format change becomes a sweep.
//
// The fix keeps Customer a pure data-and-rules type and moves each format into
// its own serializer/repository. Adding a format is a new class, not an edit.

#include <iostream>
#include <string>

// ---------- the entity knows everything ----------
namespace bad {

class Customer {
public:
    Customer(std::string name, int loyaltyPoints)
        : name_(std::move(name)), points_(loyaltyPoints) {}

    // Domain rule -- legitimately here.
    bool isGold() const { return points_ >= 1000; }

    // Persistence + wire formats -- three other actors, baked into the entity.
    std::string toJson() const {
        return "{\"name\":\"" + name_ + "\",\"points\":" +
               std::to_string(points_) + "}";
    }
    std::string toXml() const {
        return "<customer name='" + name_ + "' points='" +
               std::to_string(points_) + "'/>";
    }
    void saveToDb() const {
        std::cout << "[bad] db: insert " << name_ << "\n";
    }

private:
    std::string name_;
    int points_;
};

} // namespace bad

// ---------- pure entity, formats live outside ----------
namespace good {

// Just data and business rules. No library or database dependency.
class Customer {
public:
    Customer(std::string name, int loyaltyPoints)
        : name_(std::move(name)), points_(loyaltyPoints) {}

    const std::string& name() const { return name_; }
    int points() const { return points_; }
    bool isGold() const { return points_ >= 1000; }

private:
    std::string name_;
    int points_;
};

// Each format owns exactly one concern and changes on its own.
class JsonSerializer {
public:
    std::string serialize(const Customer& c) const {
        return "{\"name\":\"" + c.name() + "\",\"points\":" +
               std::to_string(c.points()) + "}";
    }
};

class XmlSerializer {
public:
    std::string serialize(const Customer& c) const {
        return "<customer name='" + c.name() + "' points='" +
               std::to_string(c.points()) + "'/>";
    }
};

class CustomerRepository {
public:
    void save(const Customer& c) const {
        std::cout << "[good] db: insert " << c.name() << "\n";
    }
};

} // namespace good

int main() {
    bad::Customer legacy("Ada", 1200);
    std::cout << "[bad] " << legacy.toJson() << "\n";
    legacy.saveToDb();

    std::cout << "---\n";

    good::Customer c("Ada", 1200);
    good::JsonSerializer json;
    good::XmlSerializer xml;
    good::CustomerRepository repo;

    std::cout << "[good] gold? " << (c.isGold() ? "yes" : "no") << "\n";
    std::cout << "[good] json: " << json.serialize(c) << "\n";
    std::cout << "[good] xml:  " << xml.serialize(c) << "\n";
    repo.save(c);
}
