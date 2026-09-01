#include <random>
#include <iostream>
#include <iomanip>
#include <map>

int main() {
    std::mt19937 gen(99);

    // normal_distribution: a bell curve around a mean with a given spread.
    std::normal_distribution<double> heights(170.0, 8.0);   // cm, stddev 8
    std::cout << std::fixed << std::setprecision(1) << "five heights:";
    for (int i = 0; i < 5; ++i) std::cout << ' ' << heights(gen);
    std::cout << "\n";

    // bernoulli_distribution: a weighted coin, true with probability p.
    std::bernoulli_distribution coin(0.3);   // 30% true
    int heads = 0;
    for (int i = 0; i < 1000; ++i) if (coin(gen)) ++heads;
    std::cout << "heads out of 1000 (p=0.3): " << heads << "\n";

    // discrete_distribution: pick an index in proportion to weights. Here index
    // 2 is four times as likely as index 0.
    std::discrete_distribution<int> pick({1, 2, 4, 1});
    std::map<int, int> counts;
    for (int i = 0; i < 1000; ++i) ++counts[pick(gen)];
    std::cout << "weighted picks {1,2,4,1}:";
    for (const auto& kv : counts) std::cout << ' ' << kv.first << "=>" << kv.second;
    std::cout << "\n";
}
