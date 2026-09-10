export module greeter;

// ANTI-PATTERN (described): without `export`, this function is private to the
// module, so an importer cannot call it:
//
//     const char* greet() { return "hello"; }
//     // then in the consumer:  greet();   // error: 'greet' was not declared
//
// A name in a module interface is exported ONLY if you say so -- the default is
// private, the opposite of a header, where everything is visible.

// FIX: export the names that are meant to be public.
export const char* greet() {
    return "hello from the module";
}
