#include <iostream>
#include <span>
#include <vector>

// Anti-pattern (described, not run): returning a span into a LOCAL container.
//   std::span<int> bad() {
//       std::vector<int> local{1, 2, 3};
//       return local;   // local dies here -> the returned span dangles
//   }

// A type that STORES a span is non-owning: the data it views must outlive it.
struct Window {
    std::span<const int> view; // borrows; the data lives elsewhere
};

int main() {
    // Correct: the owner outlives the Window, so the stored span stays valid.
    std::vector<int> data{5, 10, 15};
    Window w{data};
    long total = 0;
    for (int x : w.view) total += x;
    std::cout << "sum via stored span: " << total << '\n';

    // To KEEP the data past the owner's life, copy it into an owning container
    // -- a span cannot do this, because it never owns.
    std::vector<int> owned(w.view.begin(), w.view.end());
    data.clear(); // the original owner is emptied...
    long ownedSum = 0;
    for (int x : owned) ownedSum += x;
    std::cout << "owned copy still valid: size " << owned.size()
              << ", sum " << ownedSum << '\n';
    return 0;
}
