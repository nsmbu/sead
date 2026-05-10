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

#endif // SEAD_TYPES_H_
