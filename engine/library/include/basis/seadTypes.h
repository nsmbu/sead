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

#ifndef forceinline
    #define forceinline __attribute__((always_inline))
#endif // forceinline

#else

#error "Unknown platform"

#endif // cafe

#ifdef __ghs__
    #define noinline  __noinline
#else
    #define noinline
#endif // __ghs__

#endif // SEAD_TYPES_H_
