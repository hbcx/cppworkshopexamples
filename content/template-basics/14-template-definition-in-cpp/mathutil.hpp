#pragma once

// DECLARATION only. The definition lives in mathutil.cpp -- the setup that causes
// the classic template link error, UNLESS mathutil.cpp explicitly instantiates the
// types actually used (see there).
template <class T>
T twice(T x);
