#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

class Player {
public:
    explicit Player(std::string name) : name_(std::move(name)) {
        std::cout << name_ << " joined\n";
    }
    ~Player() { std::cout << name_ << " left\n"; }
    const std::string& name() const { return name_; }
private:
    std::string name_;
};

int main() {
    std::vector<std::shared_ptr<Player>> roster;   // one index...
    std::vector<std::shared_ptr<Player>> team;     // ...and a second over the same objects

    auto p = std::make_shared<Player>("Ann");
    roster.push_back(p);
    team.push_back(p);
    std::cout << p->name() << " use_count = " << p.use_count() << '\n';  // 3: p, roster, team

    p.reset();                    // our local handle goes; two container owners remain
    std::cout << "after local reset, owners = " << roster[0].use_count() << '\n';   // 2

    roster.clear();               // removed from the roster; the team still holds it
    std::cout << "after roster.clear, owners = " << team[0].use_count() << '\n';     // 1
    std::cout << "still alive: " << team[0]->name() << '\n';

    team.clear();                 // last owner gone -> Player is destroyed
    std::cout << "team cleared\n";
    return 0;
}
