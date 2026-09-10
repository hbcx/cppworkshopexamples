#include <iostream>

struct Source {
    int value;
    Source() : value(10) {}
};

struct Derived {
    int doubled;
    Derived();          // defined below, after g_source is declared
};

// ANTI-PATTERN: the static initialization order fiasco. g_derived's constructor
// reads g_source, but g_derived is defined FIRST, so it is dynamically initialized
// before g_source's constructor has run. At that moment g_source has only been
// zero-initialized, so g_derived reads value 0 and computes 0 instead of 20.
// Across translation units the order is unspecified, so this can strike even when
// the definitions look correctly ordered. (Reading the zero-initialized value is
// defined behaviour -- just the wrong value.)
Derived g_derived;
Source  g_source;

Derived::Derived() : doubled(g_source.value * 2) {}

// FIX: a Meyers singleton. The dependency is constructed on first use, so it is
// always ready before anyone reads it, whatever the definition or link order.
Source& source() {
    static Source instance;
    return instance;
}
int derivedValue() {
    return source().value * 2;
}

int main() {
    std::cout << "broken g_derived.doubled = " << g_derived.doubled << "  (wanted 20)\n";
    std::cout << "fixed  derivedValue()    = " << derivedValue() << "  (wanted 20)\n";
    return 0;
}
