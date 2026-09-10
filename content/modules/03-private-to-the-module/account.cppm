export module account;

// NOT exported: an internal helper, private to this module. Importers cannot see
// it, so it can change or disappear without affecting any caller.
int applyFee(int amount) {
    return amount + 1;   // a flat fee
}

// Only this is exported. It uses the private helper internally.
export int totalCost(int amount) {
    return amount + applyFee(amount);
}
