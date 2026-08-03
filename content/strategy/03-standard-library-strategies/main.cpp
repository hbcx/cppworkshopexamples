#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct Player {
    std::string name;
    int score;
};

int main() {
    std::vector<Player> players = {{"ada", 30}, {"bo", 50}, {"cy", 40}};

    // std::sort takes a COMPARATOR -- a strategy for ordering, injected as a
    // callable. sort's own logic never changes; the comparator decides the order,
    // so swapping it swaps the behaviour. This is Strategy, and the standard
    // library is full of it (comparators, hashers, predicates, deleters).
    std::sort(players.begin(), players.end(),
              [](const Player& a, const Player& b) { return a.score > b.score; });   // by score, descending
    std::cout << "by score: ";
    for (const auto& p : players) std::cout << p.name << "(" << p.score << ") ";
    std::cout << "\n";

    std::sort(players.begin(), players.end(),
              [](const Player& a, const Player& b) { return a.name < b.name; });     // by name, ascending
    std::cout << "by name:  ";
    for (const auto& p : players) std::cout << p.name << "(" << p.score << ") ";
    std::cout << "\n";
    return 0;
}
