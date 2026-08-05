// Anti-pattern: a subtype that weakens a postcondition.
//
// Ranking guarantees a descending-sorted result, and best() trusts that the
// front element is the highest. FastRanking returns the scores unsorted to save
// time -- it promises LESS about its output than the base -- so best() returns
// the wrong answer, silently, with no error.
//
// The fix: a subtype must keep every guarantee of the base. Optimise the HOW,
// not the promise.

#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

// ---------- FastRanking drops the sort the base promised ----------
namespace bad {

class Ranking {
public:
    virtual ~Ranking() {}
    // postcondition: the result is sorted in descending order.
    virtual std::vector<int> ordered(std::vector<int> scores) const {
        std::sort(scores.begin(), scores.end(), std::greater<int>());
        return scores;
    }
};

class FastRanking : public Ranking {
public:
    // WEAKENED postcondition: returns the scores as-is, not sorted.
    std::vector<int> ordered(std::vector<int> scores) const override {
        return scores;
    }
};

// Relies on the postcondition: the first element is the highest score.
int best(const Ranking& r, std::vector<int> scores) {
    std::vector<int> o = r.ordered(scores);
    return o.empty() ? -1 : o.front();
}

} // namespace bad

// ---------- FastRanking keeps the guarantee ----------
namespace good {

class Ranking {
public:
    virtual ~Ranking() {}
    virtual std::vector<int> ordered(std::vector<int> scores) const {
        std::sort(scores.begin(), scores.end(), std::greater<int>());
        return scores;
    }
};

class FastRanking : public Ranking {
public:
    // A different algorithm is fine -- the RESULT must still be sorted.
    std::vector<int> ordered(std::vector<int> scores) const override {
        std::sort(scores.begin(), scores.end(), std::greater<int>());
        return scores;
    }
};

int best(const Ranking& r, std::vector<int> scores) {
    std::vector<int> o = r.ordered(scores);
    return o.empty() ? -1 : o.front();
}

} // namespace good

int main() {
    std::vector<int> scores;
    scores.push_back(40);
    scores.push_back(90);
    scores.push_back(70);

    bad::Ranking baseR;
    bad::FastRanking fastR;
    std::cout << "[bad] base best = " << bad::best(baseR, scores) << "\n";  // 90
    std::cout << "[bad] fast best = " << bad::best(fastR, scores) << "\n";  // 40, wrong

    good::FastRanking gfast;
    std::cout << "[good] fast best = " << good::best(gfast, scores) << "\n"; // 90
}
