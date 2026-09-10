// The GLOBAL MODULE FRAGMENT: it starts with a bare `module;` and is the one place
// a module unit may #include a legacy header. Its declarations stay attached to
// the global module, so they do NOT leak to importers of this module.
module;
#include <cmath>

export module stats;

export double rootMeanSquare(double a, double b) {
    return std::sqrt((a * a + b * b) / 2.0);
}
