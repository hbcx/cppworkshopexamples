#include <algorithm>
#include <ranges>
#include <string>
#include <vector>
#include <iostream>

struct City {
    std::string name;
    int population;
};

int main() {
    std::vector<City> cities{
        {"Oslo", 700}, {"Rome", 2800}, {"Bern", 130}, {"Kyiv", 2900},
        {"Riga", 600}, {"Graz", 290}};

    // ranges::partial_sort takes the range, the middle iterator, a comparator, and a
    // PROJECTION, so top-k by a member needs no hand-written comparator. Here the 3
    // most populous cities, largest first, ordered by population.
    auto mid = cities.begin() + 3;
    std::ranges::partial_sort(cities, mid, std::ranges::greater{}, &City::population);

    std::cout << "3 biggest cities:\n";
    for (auto it = cities.begin(); it != mid; ++it)
        std::cout << "  " << it->name << " (" << it->population << ")\n";
    return 0;
}
