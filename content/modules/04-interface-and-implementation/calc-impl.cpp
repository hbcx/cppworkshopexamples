// An IMPLEMENTATION unit of module calc: `module calc;` with no export. It sees
// everything in the interface and provides the definitions. Editing it does not
// force consumers to rebuild against a changed interface.
module calc;

int add(int a, int b) {
    return a + b;
}

int mul(int a, int b) {
    return a * b;
}
