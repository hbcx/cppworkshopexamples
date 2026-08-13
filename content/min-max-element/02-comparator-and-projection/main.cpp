#include <algorithm>
#include <string>
#include <vector>
#include <iostream>

struct Player {
    std::string name;
    int score;
};

int main() {
    std::vector<std::string> words{"red", "green", "sky", "purple", "blue"};

    // A comparator lets min/max_element rank by any rule. Here, the LONGEST word:
    // compare by size instead of comparing the strings themselves.
    auto longest = std::max_element(words.begin(), words.end(),
        [](const std::string& a, const std::string& b) { return a.size() < b.size(); });
    std::cout << "longest word: " << *longest << '\n';

    // The same for structs: the player with the highest score. max_element still
    // returns an iterator, so reach through it for the fields.
    std::vector<Player> players{{"Ada", 40}, {"Ben", 75}, {"Cid", 60}};
    auto best = std::max_element(players.begin(), players.end(),
        [](const Player& a, const Player& b) { return a.score < b.score; });
    std::cout << "top scorer: " << best->name << " with " << best->score << '\n';
    return 0;
}
