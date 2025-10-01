#ifndef SEAD_FRAME_HEAP_H_
#define SEAD_FRAME_HEAP_H_

#include <heap/seadHeap.h>

namespace sead {

class FrameHeap : public Heap
{
    SEAD_RTTI_OVERRIDE(FrameHeap, Heap)

public:
    FrameHeap(const SafeString& name, Heap* parent, void* start, u32 size, HeapDirection direction, bool enable_lock);
    virtual ~FrameHeap();

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
    size_t getMaxAllocatableSize(s32 alignment = 4) const override;
    bool isInclude(const void*) const override;
    bool isFreeable() const override;
    bool isResizable() const override;
    bool isAdjustable() const override;

public:
    static FrameHeap* tryCreate(u32 size, const SafeString& name, Heap* parent = nullptr, HeapDirection direction = cHeapDirection_Forward, bool enable_lock = false);

protected:
    void initialize_();

protected:
    class State
    {
        void* mHeadPtr;
        void* mTailPtr;

        friend class FrameHeap;
    };

    State state;
};
#ifdef cafe
static_assert(sizeof(FrameHeap) == 0x98, "sead::FrameHeap size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_FRAME_HEAP_H_
