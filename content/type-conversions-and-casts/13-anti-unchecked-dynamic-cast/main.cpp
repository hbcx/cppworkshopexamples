#include <iostream>

struct Animal { virtual ~Animal() = default; };
struct Dog : Animal { void fetch() const { std::cout << "fetch!\n"; } };
struct Cat : Animal {};

// BAD: assumes the cast succeeded. If a is not a Dog, d is nullptr and d->fetch()
// dereferences null -- undefined behavior. (Called below only on a real Dog, so
// this program stays defined; the hazard is calling it on anything else.)
void handleUnchecked(Animal* a) {
    Dog* d = dynamic_cast<Dog*>(a);
    d->fetch();                        // no null check -- the bug
}

// GOOD: branch on the result, so a non-Dog is handled instead of dereferenced.
void handleChecked(Animal* a) {
    if (Dog* d = dynamic_cast<Dog*>(a))
        d->fetch();
    else
        std::cout << "not a Dog, skipped safely\n";
}

int main() {
    Dog dog;
    Cat cat;

    handleUnchecked(&dog);   // fetch!   (works ONLY because it is really a Dog)

    handleChecked(&dog);     // fetch!
    handleChecked(&cat);     // not a Dog, skipped safely   (the unchecked version would crash here)
    return 0;
}
