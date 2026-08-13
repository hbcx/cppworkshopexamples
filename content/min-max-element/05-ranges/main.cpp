#include <algorithm>
#include <string>
#include <vector>
#include <iostream>

struct City {
    std::string name;
    int population;
};

int main() {
    std::vector<City> cities{
        {"Oslo", 700}, {"Rome", 2800}, {"Bern", 130}, {"Kyiv", 2900}};

    // ranges::max_element takes the whole range and an optional PROJECTION, so you
    // rank by a member without writing a comparator: project each City to its
    // population and it compares those. The result is still an iterator.
    auto biggest = std::ranges::max_element(cities, {}, &City::population);
    std::cout << "biggest city: " << biggest->name << '\n';

    // ranges::min and ranges::max return the VALUE (not an iterator) and take a
    // projection too. Here, the city with the smallest population, by value.
    City smallest = std::ranges::min(cities, {}, &City::population);
    std::cout << "smallest city: " << smallest.name << '\n';

    // ranges::minmax returns a {min, max} pair of VALUES in one pass.
    auto [lo, hi] = std::ranges::minmax(std::vector<int>{5, 2, 9, 2, 9});
    std::cout << "minmax values: " << lo << ' ' << hi << '\n';
    return 0;
}
