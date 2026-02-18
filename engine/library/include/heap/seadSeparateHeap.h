#ifndef SEAD_SEPARATE_HEAP_H_
#define SEAD_SEPARATE_HEAP_H_

#include <heap/seadHeap.h>

namespace sead {

// TODO

class SeparateHeap : public Heap
{
public:
    SeparateHeap(const SafeString& name, Heap* parent, void* management_area, u32 management_area_size, void* allocate_area, u32 allocate_area_size, bool enable_lock);
    ~SeparateHeap() override;

    // TODO: These should be inline
    const void* getStartAddress() const override;
    const void* getEndAddress() const override;
    size_t getSize() const override;

public:
    static SeparateHeap* create(const SafeString& name, u32 management_area_size, u32 allocate_area_size, Heap* parent = nullptr, bool enable_lock = false);
};

}

#endif // SEAD_SEPARATE_HEAP_H_
