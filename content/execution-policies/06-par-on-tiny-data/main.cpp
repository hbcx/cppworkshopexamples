#include <execution>
#include <iostream>
#include <numeric>
#include <vector>

int main() {
    // A tiny vector and a trivial per-element operation.
    std::vector<int> small{1, 2, 3, 4, 5, 6, 7, 8};

    // ANTI-PATTERN: reach for par by reflex. Parallelism is not free: splitting
    // the range, launching or waking worker threads, and merging results all cost
    // time. For a handful of elements, or an operation as cheap as an add, that
    // fixed overhead dwarfs the actual work, so par is SLOWER than plain seq --
    // sometimes by a lot. The result is identical; only the cost differs.
    long long parSum = std::reduce(std::execution::par, small.begin(), small.end(), 0LL);

    // RIGHT CHOICE for small or cheap work: seq (or the plain overload). No thread
    // setup, no merge -- just the loop.
    long long seqSum = std::reduce(std::execution::seq, small.begin(), small.end(), 0LL);

    std::cout << "same result: " << (parSum == seqSum ? "yes" : "no")
              << " (" << seqSum << ")\n";
    std::cout << "for 8 cheap adds, seq is the right call -- par only pays off\n"
              << "when the data is large AND the per-element work is non-trivial\n";

    // Rule of thumb: parallelize when N is big (tens of thousands and up) and each
    // element does real work; measure rather than assume, and default to seq for
    // small inputs. par on tiny data is a pessimization dressed as an optimization.
    return 0;
}
