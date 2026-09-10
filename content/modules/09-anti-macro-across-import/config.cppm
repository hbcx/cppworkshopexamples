export module config;

// A macro is a preprocessor construct and is NOT exported by a module. It is
// usable inside this unit, but importers never see it -- unlike a header, where
// a #define leaks to every includer.
#define SECRET_ANSWER 42

// ANTI-PATTERN (described): a consumer expecting the macro to arrive with import
// is disappointed:
//
//     import config;
//     int x = SECRET_ANSWER;   // error: 'SECRET_ANSWER' was not declared
//
// FIX: export a real entity -- a function or a constant -- instead of relying on
// a macro to cross the boundary.
export int answer() {
    return SECRET_ANSWER;   // fine: the macro is visible HERE, inside the module
}

export inline constexpr int answer_constant = SECRET_ANSWER;
