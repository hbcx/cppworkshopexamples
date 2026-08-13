#include <algorithm>
#include <string>
#include <vector>
#include <iostream>

struct Player {
    std::string name;
    int score;
};

int main() {
    std::vector<Player> players{
        {"Ada", 40}, {"Ben", 88}, {"Cid", 61}, {"Dan", 95}, {"Eve", 73}};

    // Top 2 by score: partial_sort with a comparator that orders by score DESCENDING
    // puts the two highest scorers first, in order. The comparator flips "smallest
    // first" into "largest first", so the k smallest under it are the k largest.
    const std::size_t k = 2;
    std::partial_sort(players.begin(), players.begin() + k, players.end(),
                      [](const Player& a, const Player& b) { return a.score > b.score; });

    std::cout << "top 2 scorers:\n";
    for (std::size_t i = 0; i < k; ++i)
        std::cout << "  " << players[i].name << " (" << players[i].score << ")\n";
    return 0;
}
