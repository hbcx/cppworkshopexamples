#pragma once

// The measurement harness shared by the benchmark examples.
//
// Three traps live in here, and every one of them silently produces a plausible
// number rather than an error:
//
//   1. Dead code. At -O2 a loop whose result nobody reads is dead, and the
//      optimizer deletes it. The benchmark then reports a few nanoseconds --
//      the cost of the clock calls -- and you conclude the container is free.
//      do_not_optimize() below is the barrier that prevents it.
//   2. Clock granularity. 100 lookups take about 2 us; steady_clock's tick is
//      coarse enough that you would be measuring the timer. measure() therefore
//      repeats the body until a single sample lasts at least kMinSampleNs, and
//      divides afterwards.
//   3. Teardown. Destroying a million-node std::map costs real time. If the
//      clock is still running, that cost lands on the BUILD figure. measure_build()
//      stops the clock before the container dies.

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <vector>

namespace bench {

// steady_clock, not high_resolution_clock: the latter is permitted to be an
// alias for system_clock, which is not monotonic -- an NTP correction mid-run
// can hand you a negative duration.
using Clock = std::chrono::steady_clock;

// Force `value` to be materialized, so the computation that produced it cannot
// be optimized away. The "m" constraint puts it in memory and the "memory"
// clobber tells the compiler the asm block may read anything -- an empty asm
// body it cannot see through.
template <class T>
inline void do_not_optimize(const T& value) {
    __asm__ __volatile__("" : : "m"(value) : "memory");
}

struct Stats {
    double median_ns;  // per operation
    double min_ns;     // per operation
};

// 7 samples: enough for a stable median, cheap enough to run the whole grid.
inline constexpr int kSamples = 7;
// 2 ms per sample -- three orders of magnitude above the clock's tick.
inline constexpr double kMinSampleNs = 2'000'000.0;
inline constexpr std::size_t kMaxInner = 1u << 20;

inline Stats summarize(std::vector<double> per_op) {
    std::sort(per_op.begin(), per_op.end());
    // Median AND minimum, deliberately -- not a trimmed mean.
    //
    // Timing noise is one-sided: a scheduler preemption, an interrupt or a
    // migration to a cold core can only ADD time, never remove it. So the
    // minimum is the sample least contaminated by the machine and sits closest
    // to the true cost, while the median is the robust typical case. Printing
    // both lets the reader see how noisy the run was: if they are far apart,
    // the machine was busy and the numbers deserve less trust. An average --
    // even one with the extremes trimmed -- just blends the noise back in.
    return Stats{per_op[per_op.size() / 2], per_op.front()};
}

namespace detail {

template <class Body>
double one_sample(std::size_t inner, Body& body) {
    const auto t0 = Clock::now();
    for (std::size_t i = 0; i < inner; ++i) {
        auto result = body();
        do_not_optimize(result);
    }
    const auto t1 = Clock::now();
    return std::chrono::duration<double, std::nano>(t1 - t0).count();
}

}  // namespace detail

// Steady-state measurement. `body` performs `ops` operations against an ALREADY
// BUILT structure and returns a checksum; returning it is what keeps the work
// alive past the optimizer. Result is nanoseconds per single operation.
template <class Body>
Stats measure(std::size_t ops, Body body) {
    // Calibrate the inner repeat count until one sample is long enough to
    // measure. This doubles as the warm-up: caches and pages are hot by the end.
    std::size_t inner = 1;
    while (inner < kMaxInner) {
        if (detail::one_sample(inner, body) >= kMinSampleNs) {
            break;
        }
        inner *= 2;
    }

    std::vector<double> per_op;
    per_op.reserve(kSamples);
    for (int i = 0; i < kSamples; ++i) {
        const double ns = detail::one_sample(inner, body);
        per_op.push_back(ns / static_cast<double>(inner * ops));
    }
    return summarize(std::move(per_op));
}

// Construction measurement. `make` returns a fresh container built from `ops`
// elements. Only the call is timed: the container is destroyed after the clock
// has stopped, at the end of the iteration, so teardown never lands on the
// build cost.
template <class Make>
Stats measure_build(std::size_t ops, Make make) {
    std::vector<double> per_op;
    per_op.reserve(kSamples);
    for (int i = 0; i < kSamples + 1; ++i) {
        const auto t0 = Clock::now();
        auto container = make();
        const auto t1 = Clock::now();
        do_not_optimize(container);
        if (i > 0) {  // first iteration is the warm-up, thrown away
            const double ns =
                std::chrono::duration<double, std::nano>(t1 - t0).count();
            per_op.push_back(ns / static_cast<double>(ops));
        }
    }
    return summarize(std::move(per_op));
}

}  // namespace bench
