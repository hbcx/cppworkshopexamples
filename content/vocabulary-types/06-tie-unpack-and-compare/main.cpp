#include <iostream>
#include <string>
#include <tuple>    // std::tie, std::ignore
#include <utility>

// A record whose ordering must go by last name, then first name, then id.
struct Person {
    std::string last;
    std::string first;
    int id;
};

// Comparing two ties compares the referenced members lexicographically -- the
// idiomatic multi-key operator< without a hand-written if-chain.
bool operator<(const Person& a, const Person& b) {
    return std::tie(a.last, a.first, a.id) < std::tie(b.last, b.first, b.id);
}

int main() {
    Person x{"Lovelace", "Ada", 2};
    Person y{"Lovelace", "Ada", 5};
    // Tie on last and first, so the id breaks it: 2 < 5.
    std::cout << "x < y? " << (x < y ? "yes" : "no") << '\n';

    // std::tie also unpacks a pair into variables that already exist.
    std::pair<int, std::string> row{7, "seven"};
    int n;
    std::string word;
    std::tie(n, word) = row;
    std::cout << "n = " << n << ", word = " << word << '\n';

    // std::ignore drops a field you do not want.
    std::tie(n, std::ignore) = std::make_pair(99, "unused");
    std::cout << "n now " << n << " (second ignored)\n";
    return 0;
}
