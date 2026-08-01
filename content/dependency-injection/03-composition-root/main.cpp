#include <iostream>
#include <memory>
#include <string>

// --- interfaces ---
struct UserRepository {
    virtual std::string nameOf(int id) = 0;
    virtual ~UserRepository() = default;
};

struct Mailer {
    virtual void mail(const std::string& who, const std::string& what) = 0;
    virtual ~Mailer() = default;
};

// --- implementations ---
struct InMemoryUsers : UserRepository {
    std::string nameOf(int id) override {
        return id == 1 ? "alice" : "unknown";
    }
};

struct ConsoleMailer : Mailer {
    void mail(const std::string& who, const std::string& what) override {
        std::cout << "mail -> " << who << ": " << what << '\n';
    }
};

// A high-level service that depends only on the interfaces, both injected. It
// does not know or care which implementations it received.
class Welcomer {
public:
    Welcomer(std::unique_ptr<UserRepository> users, std::unique_ptr<Mailer> mailer)
        : users_(std::move(users)), mailer_(std::move(mailer)) {}
    void welcome(int userId) {
        mailer_->mail(users_->nameOf(userId), "Welcome aboard!");
    }
private:
    std::unique_ptr<UserRepository> users_;   // owning injection
    std::unique_ptr<Mailer> mailer_;
};

int main() {
    // The composition root: the ONE place that names concrete types and wires
    // the graph. Swapping an implementation is a one-line change here; nothing
    // in Welcomer is touched.
    Welcomer welcomer(std::make_unique<InMemoryUsers>(),
                      std::make_unique<ConsoleMailer>());

    welcomer.welcome(1);
    welcomer.welcome(2);
    return 0;
}
