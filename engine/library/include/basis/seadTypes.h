#ifndef SEAD_TYPES_H_
#define SEAD_TYPES_H_

#ifdef cafe

#include <cstddef>
#include <types.h>

typedef unsigned long uintptr;
typedef long sintptr;
typedef s64 d_sintptr;

typedef uintptr uintptr_t;
typedef sintptr  intptr_t;

#else

#error "Unknown platform"

#endif // cafe

// TODO: Move to the proper headers
#define SEAD_MACRO_UTIL_ROUNDUP(x, y) ((x) + ((y) - 1) & ~((y) - 1))
#define SEAD_MACRO_UTIL_ROUNDDOWN(x, y) ((x) & ~((y) - 1))

#endif // SEAD_TYPES_H_
