// The INTERFACE unit: it declares and exports the API, but leaves the bodies to
// an implementation unit. Callers only ever see this.
export module calc;

export int add(int a, int b);
export int mul(int a, int b);
