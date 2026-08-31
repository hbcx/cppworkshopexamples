// The sieve of Eratosthenes wants one boolean per number, and a bitset packs
// them eight to a byte. Bit i means "i is composite"; the bits left unset are
// the primes. N is a compile-time constant, which is exactly what bitset needs.

#include <bitset>
#include <iostream>

int main() {
    const std::size_t N = 50;
    std::bitset<N> composite;          // all false: assume prime until marked

    for (std::size_t i = 2; i * i < N; ++i) {
        if (!composite[i]) {           // i is prime
            for (std::size_t j = i * i; j < N; j += i)
                composite.set(j);      // mark every multiple of i
        }
    }

    std::cout << "primes under " << N << ':';
    std::size_t primes = 0;
    for (std::size_t i = 2; i < N; ++i) {
        if (!composite[i]) { std::cout << ' ' << i; ++primes; }
    }
    std::cout << "\ncount: " << primes << '\n';
}
