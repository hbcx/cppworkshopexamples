#include <iostream>
#include <random>

int main() {
    // std::mt19937 has a large internal state (624 32-bit words). Seeding it
    // from a SINGLE number leaves most of that state derived from one value --
    // fine for a toy, weak for anything real. The right way is std::seed_seq,
    // which spreads entropy across the whole state. In production you feed the
    // seed_seq from std::random_device (a hardware/OS entropy source):
    std::random_device rd;
    std::seed_seq realSeed{rd(), rd(), rd(), rd()};
    std::mt19937 good(realSeed);

    // Draw from the well-seeded engine. Its output is different every run, so
    // we do not print the value -- we only confirm it lands in the asked range.
    std::uniform_int_distribution<int> d100(1, 100);
    int roll = d100(good);
    std::cout << "well-seeded roll in [1,100]? " << std::boolalpha
              << (roll >= 1 && roll <= 100) << '\n';

    // For a REPRODUCIBLE run (tests, replays) seed from fixed numbers instead of
    // random_device -- same seed_seq, same sequence, every time.
    std::seed_seq fixedSeed{1, 2, 3, 4};
    std::mt19937 engine(fixedSeed);

    // Reuse ONE engine across draws and across distributions. Each call advances
    // its state, so you keep getting fresh numbers -- do not build a new engine
    // per call (that would repeat the same value forever).
    std::uniform_int_distribution<int> dice(1, 6);
    std::cout << "three dice:";
    for (int i = 0; i < 3; ++i) std::cout << ' ' << dice(engine);
    std::cout << '\n';

    std::uniform_int_distribution<int> coin(0, 1);
    std::cout << "one coin:  " << coin(engine) << '\n';
    return 0;
}
