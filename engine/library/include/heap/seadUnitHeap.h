#ifndef SEAD_UNIT_HEAP_H_
#define SEAD_UNIT_HEAP_H_

#include <container/seadFreeList.h>
#include <heap/seadHeap.h>

namespace sead {

class UnitHeap : public Heap
{
    SEAD_RTTI_OVERRIDE(UnitHeap, Heap)

public:
    UnitHeap(const SafeString& name, Heap* parent, void* address, u32 size, u32, bool);
    ~UnitHeap() override;

    void destroy() override;
    size_t adjust() override;
    void* tryAlloc(size_t size, s32 alignment) override;
    void free(void* ptr) override;
    void* resizeFront(void*, size_t) override;
    void* resizeBack(void*, size_t) override;
    void freeAll() override;
    const void* getStartAddress() const override;
    const void* getEndAddress() const override;
    size_t getSize() const override;
    size_t getFreeSize() const override;
    size_t getMaxAllocatableSize(int) const override;
    bool isInclude(const void*) const override;
    bool isFreeable() const override;
    bool isResizable() const override;
    bool isAdjustable() const override;

public:
    static UnitHeap* tryCreate(u32 size, const SafeString& name, u32 block_size, s32 alignment = 4, Heap* parent = nullptr, bool enable_lock = false);
    static UnitHeap* tryCreateWithBlockNum(u32 block_size, u32 num, const SafeString& name, s32 alignment = 4, Heap* parent = nullptr, bool enable_lock = false);

protected:
    u32 mBlockSize;
    void* mAreaStart;
    u32 mAreaSize;
    u32 mFreeSize;
    FreeList mFreeList;
};
#ifdef cafe
static_assert(sizeof(UnitHeap) == 0xA8, "sead::UnitHeap size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_UNIT_HEAP_H_
