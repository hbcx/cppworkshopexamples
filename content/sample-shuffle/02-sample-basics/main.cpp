#include <algorithm>
#include <iostream>
#include <iterator>
#include <random>
#include <vector>

int main() {
    // A population of 10 items. We want 3 of them, chosen at random, with no
    // item picked twice (sampling WITHOUT replacement).
    std::vector<int> population{10, 20, 30, 40, 50, 60, 70, 80, 90, 100};

    std::vector<int> picked;
    std::mt19937 engine(777);

    // std::sample copies n randomly chosen elements from [first, last) into the
    // output. Each element is equally likely to be chosen, and -- unlike shuffle
    // -- the chosen elements keep their ORIGINAL relative order in the output.
    std::sample(population.begin(), population.end(),
                std::back_inserter(picked), 3, engine);

    std::cout << "sampled 3:";
    for (int x : picked) std::cout << ' ' << x;
    std::cout << '\n';

    // The output preserves input order, so a sample of a sorted range stays
    // sorted -- here the picks come out ascending, matching the population.
    std::cout << "still ascending? " << std::boolalpha
              << std::is_sorted(picked.begin(), picked.end()) << '\n';

    // Asking for MORE than the population size is not an error: sample simply
    // returns the whole population (still in order), never duplicating.
    std::vector<int> tooMany;
    std::sample(population.begin(), population.end(),
                std::back_inserter(tooMany), 999, engine);
    std::cout << "asked for 999, got " << tooMany.size() << '\n';
    return 0;
}
