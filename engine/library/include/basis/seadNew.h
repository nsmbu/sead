#ifndef SEAD_NEW_H_
#define SEAD_NEW_H_

#include <cstddef>
#include <new>

#include <basis/seadTypes.h>

namespace sead {

const size_t cPtrSize = sizeof(void*);
const s32 cDefaultAlignment = alignof(void*);

class Heap;

} // namespace sead

void* operator new(size_t size);
void* operator new[](size_t size);

void operator delete(void* ptr);
void operator delete[](void* ptr);

void* operator new(size_t size, sead::Heap* heap, s32 alignment = 4);
void* operator new[](size_t size, sead::Heap* heap, s32 alignment = 4);

#endif // SEAD_NEW_H_
