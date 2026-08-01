#include <iostream>
#include <string>

// Domain has four severities; the vendor protocol has only two. A two-way
// adapter between them cannot be lossless -- the mapping is not a bijection.
enum class Severity { Low, Medium, High, Critical };
enum class VendorLevel { Normal, Alert };

std::string name(Severity s) {
    switch (s) {
        case Severity::Low:      return "Low";
        case Severity::Medium:   return "Medium";
        case Severity::High:     return "High";
        case Severity::Critical: return "Critical";
    }
    return "?";
}

// Anti-pattern: treat the adapter as reversible. Mapping down to the vendor's
// two levels collapses High and Critical into Alert; mapping back cannot tell
// them apart, so a round-trip silently downgrades Critical to High.
VendorLevel toVendor(Severity s) {
    return (s == Severity::High || s == Severity::Critical)
               ? VendorLevel::Alert : VendorLevel::Normal;
}
Severity fromVendor(VendorLevel v) {
    return v == VendorLevel::Alert ? Severity::High : Severity::Low;   // lossy
}

int main() {
    Severity original = Severity::Critical;
    Severity roundTripped = fromVendor(toVendor(original));

    std::cout << "original      -> " << name(original) << '\n';
    std::cout << "round-tripped -> " << name(roundTripped) << '\n';
    std::cout << "lossless? " << (original == roundTripped ? "yes" : "no") << '\n';

    // Fix: do not model a lossy mapping as reversible. Adapt only in the
    // direction the boundary needs (domain -> vendor for sending); if you must
    // keep what the vendor cannot represent, store the original Severity
    // alongside rather than reconstructing it from the vendor level.
    return 0;
}
