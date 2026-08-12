#include <algorithm>
#include <vector>
#include <string>
#include <iostream>

struct Player {
    std::string name;
    int score;
};

static void print(const std::vector<Player>& ps, const char* title) {
    std::cout << title << '\n';
    for (const auto& p : ps) std::cout << "  " << p.name << ": " << p.score << '\n';
}

int main() {
    std::vector<Player> players{
        {"Ada", 42},
        {"Linus", 88},
        {"Grace", 65},
    };

    // Sort by a member with a comparator lambda: highest score first.
    std::sort(players.begin(), players.end(),
        [](const Player& a, const Player& b){ return a.score > b.score; });
    print(players, "by score (desc):");

    // C++20 ranges::sort with a PROJECTION: {} is the default comparator (less)
    // and &Player::name projects each element to the field to compare -- so the
    // ordering rule and the field selection stay separate. Ascending by name.
    std::ranges::sort(players, {}, &Player::name);
    print(players, "by name (asc):");
    return 0;
}
