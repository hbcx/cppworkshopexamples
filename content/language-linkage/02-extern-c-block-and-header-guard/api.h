#pragma once

/* This header works from BOTH C and C++. In C++ the declarations are wrapped in
   extern "C" so they get C linkage; in C the __cplusplus guard hides the wrapper,
   which C would not understand. This is the standard shape of a C-callable header. */
#ifdef __cplusplus
extern "C" {
#endif

int c_add(int a, int b);
int c_mul(int a, int b);

#ifdef __cplusplus
}
#endif
