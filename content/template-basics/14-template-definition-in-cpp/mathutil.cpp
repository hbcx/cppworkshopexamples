#include "mathutil.hpp"

// Definition in a .cpp. On its own this does NOT let main.cpp instantiate
// twice<int>, because implicit instantiation needs the definition visible in the
// USING translation unit -- so main.cpp would fail to LINK (undefined reference).
template <class T>
T twice(T x) { return x + x; }

// The fix when the definition must stay in a .cpp: EXPLICITLY instantiate the
// types you use. This emits their code here so the linker can find it.
template int    twice<int>(int);
template double twice<double>(double);
