#pragma once

#include <basis/seadNew.h>
#include <container/seadFreeList.h>
#include <container/seadListImpl.h>
#include <prim/seadPtrUtil.h>

namespace sead {

template <typename T>
class ObjList : public ListImpl
{
protected:
    struct Node
    {
        //SEAD_NO_COPY(Node);

        Node()
            : item()
            , link()
        {
        }

        T item;
        ListNode link;
    };

    using CompareCallback = s32 (*)(const T* a, const T* b);

public:
    ObjList()
        : ListImpl()
        , mFreeList()
        , mLimitNum(0)
    {
    }

    ObjList(s32 limitNum, void* buf)
        : ListImpl()
        , mFreeList()
        , mLimitNum(0)
    {
        setBuffer(limitNum, buf);
    }

    void allocBuffer(s32 limitNum, s32 alignment = cDefaultAlignment)
    {
        if (!tryAllocBuffer(limitNum, alignment))
        {
            //AllocFailAssert(nullptr, limitNum * sizeof(Node), alignment);
        }
    }

    void allocBuffer(s32 limitNum, Heap* heap, s32 alignment = cDefaultAlignment)
    {
        if (!tryAllocBuffer(limitNum, heap, alignment))
        {
            //AllocFailAssert(heap, limitNum * sizeof(Node), alignment);
        }
    }

    bool tryAllocBuffer(s32 limitNum, s32 alignment = cDefaultAlignment)
    {
        SEAD_ASSERT(mFreeList.work() == nullptr);

        if (limitNum <= 0)
        {
            SEAD_ASSERT_MSG(false, "limitNum[%d] must be larger than zero", limitNum);
            return false;
        }

        void* buf = new(nullptr, alignment) u8[limitNum * sizeof(Node)];
        if (!buf)
        {
            return false;
        }

        setBuffer(limitNum, buf);
        return true;
    }

    bool tryAllocBuffer(s32 limitNum, Heap* heap, s32 alignment = cDefaultAlignment)
    {
        SEAD_ASSERT(mFreeList.work() == nullptr);

        if (limitNum <= 0)
        {
            SEAD_ASSERT_MSG(false, "limitNum[%d] must be larger than zero", limitNum);
            return false;
        }

        void* buf = new(heap, alignment) u8[limitNum * sizeof(Node)];
        if (!buf)
        {
            return false;
        }

        setBuffer(limitNum, buf);
        return true;
    }

    void freeBuffer()
    {
        if (isBufferReady())
        {
            clear();

            delete[] static_cast<u8*>(mFreeList.work());

            mLimitNum = 0;
            mFreeList.cleanup();
        }
    }

    void setBuffer(s32 limitNum, void* buf)
    {
        if (limitNum <= 0)
        {
            SEAD_ASSERT_MSG(false, "limitNum[%d] must be larger than zero", limitNum);
            return;
        }

        if (!buf)
        {
            SEAD_ASSERT_MSG(false, "buf is null");
            return;
        }

        mFreeList.init(buf, sizeof(Node), limitNum);
        mLimitNum = limitNum;
    }

    bool isBufferReady() const { return mFreeList.work() != nullptr; }
    bool isFull() const { return mCount >= mLimitNum; }

    s32 maxSize() const { return mLimitNum; }

    void clear()
    {
        ListNode* node = mStartEnd.next();

        while (node != &mStartEnd)
        {
            ListNode* temp = node->next();

            ListImpl::erase(node);

            {
                T* item = listNodeToObj(node);
                item->~T();
                mFreeList.put(item);
            }

            node = temp;
        }
    }

    T* birthBack()
    {
        if (isFull())
        {
            SEAD_ASSERT_MSG(false, "buffer full.");
            return nullptr;
        }

        Node* node = new(mFreeList.get()) Node();
        ListImpl::pushBack(objToListNode(&node->item));

        return &node->item;
    }

    T* tryBirthBack()
    {
        if (isFull())
        {
            return nullptr;
        }

        Node* node = new(mFreeList.get()) Node();
        ListImpl::pushBack(objToListNode(&node->item));

        return &node->item;
    }

    T* birthFront()
    {
        if (isFull())
        {
            SEAD_ASSERT_MSG(false, "buffer full.");
            return nullptr;
        }

        Node* node = new(mFreeList.get()) Node();
        ListImpl::pushFront(objToListNode(&node->item));

        return &node->item;
    }

    T* tryBirthFront()
    {
        if (isFull())
        {
            return nullptr;
        }

        Node* node = new(mFreeList.get()) Node();
        ListImpl::pushFront(objToListNode(&node->item));

        return &node->item;
    }

    T* birthBefore(const T* basis)
    {
        if (isFull())
        {
            SEAD_ASSERT_MSG(false, "buffer full.");
            return nullptr;
        }

        Node* node = new(mFreeList.get()) Node();
        ListImpl::insertBefore(objToListNode(basis), objToListNode(&node->item));

        return &node->item;
    }

    T* tryBirthBefore(const T* basis)
    {
        if (isFull())
        {
            return nullptr;
        }

        Node* node = new(mFreeList.get()) Node();
        ListImpl::insertBefore(objToListNode(basis), objToListNode(&node->item));

        return &node->item;
    }

    T* birthAfter(const T* basis)
    {
        if (isFull())
        {
            SEAD_ASSERT_MSG(false, "buffer full.");
            return nullptr;
        }

        Node* node = new(mFreeList.get()) Node();
        ListImpl::insertAfter(objToListNode(basis), objToListNode(&node->item));

        return &node->item;
    }

    T* tryBirthAfter(const T* basis)
    {
        if (isFull())
        {
            return nullptr;
        }

        Node* node = new(mFreeList.get()) Node();
        ListImpl::insertAfter(objToListNode(basis), objToListNode(&node->item));

        return &node->item;
    }

    void erase(T* obj)
    {
        ListImpl::erase(objToListNode(obj));
        obj->~T();
        mFreeList.put(obj);
    }

    T* front() const { return listNodeToObjWithNullCheck(ListImpl::front()); }
    T* back() const { return listNodeToObjWithNullCheck(ListImpl::back()); }

    T* prev(const T* obj) const
    {
        ListNode* prev = objToListNode(obj)->prev();
        if (prev == &mStartEnd)
        {
            return nullptr;
        }

        return listNodeToObj(prev);
    }

    T* next(const T* obj) const
    {
        ListNode* next = objToListNode(obj)->next();
        if (next == &mStartEnd)
        {
            return nullptr;
        }

        return listNodeToObj(next);
    }

    T* nth(s32 index) const { return listNodeToObjWithNullCheck(ListImpl::nth(index)); }
    s32 indexOf(const T* obj) const { return ListImpl::indexOf(objToListNode(obj)); }

    bool isNodeLinked(const T* obj) const { return objToListNode(obj)->isLinked(); }

    void swap(T* obj1, T* obj2) { ListImpl::swap(objToListNode(obj1), objToListNode(obj2)); }
    void moveAfter(T* basis, T* obj) { ListImpl::moveAfter(objToListNode(basis), objToListNode(obj)); }
    void moveBefore(T* basis, T* obj) { ListImpl::moveBefore(objToListNode(basis), objToListNode(obj)); }

    void sort() { sort(&compareT); }
    void sort(CompareCallback cmp)
    {
        ListImpl::sort(offsetof(Node, link), reinterpret_cast<CompareCallbackImpl>(cmp));
    }

    void mergeSort() { mergeSort(&compareT); }
    void mergeSort(CompareCallback cmp)
    {
        ListImpl::mergeSort(offsetof(Node, link), reinterpret_cast<CompareCallbackImpl>(cmp));
    }

    T* find(const T* obj) const { return find(obj, &compareT); }
    T* find(const T* obj, CompareCallback cmp) const
    {
        return listNodeToObjWithNullCheck(ListImpl::find(obj, offsetof(Node, link), reinterpret_cast<CompareCallbackImpl>(cmp)));
    }

    void uniq() { uniq(&compareT); }
    void uniq(CompareCallback cmp)
    {
        if (mCount < 2)
        {
            return;
        }

        for (ListNode* node = mStartEnd.next(); node != &mStartEnd; node = node->next())
        {
            ListNode* checkNode = node->next();
            while (checkNode != &mStartEnd)
            {
                if (cmp(listNodeToObj(node), listNodeToObj(checkNode)) == 0)
                {
                    ListNode* temp = checkNode->next();
                    erase(listNodeToObj(checkNode));
                    checkNode = temp;
                }
                else
                {
                    checkNode = checkNode->next();
                }
            }
        }
    }

public:
    class iterator
    {
    public:
        explicit iterator(T* ptr)
            : mPtr(ptr)
        {
        }

        iterator& operator++()
        {
            ListNode* next = static_cast<ListNode*>(PtrUtil::addOffset(mPtr, offsetof(Node, link)))->next();
            mPtr = static_cast<T*>(PtrUtil::addOffset(next, -static_cast<intptr_t>(offsetof(Node, link))));
            return *this;
        }

        T& operator*() const { return *mPtr; }
        T* operator->() const { return mPtr; }

        friend bool operator==(const iterator& lhs, const iterator& rhs) { return lhs.mPtr == rhs.mPtr; }
        friend bool operator!=(const iterator& lhs, const iterator& rhs) { return lhs.mPtr != rhs.mPtr; }

    private:
        T* mPtr;

        friend class constIterator;
    };

    class constIterator
    {
    public:
        explicit constIterator(const T* ptr)
            : mPtr(ptr)
        {
        }

        constIterator(const iterator& it)
            : mPtr(it.mPtr)
        {
        }

        constIterator& operator++()
        {
            const ListNode* next = static_cast<const ListNode*>(PtrUtil::addOffset(mPtr, offsetof(Node, link)))->next();
            mPtr = static_cast<const T*>(PtrUtil::addOffset(next, -static_cast<intptr_t>(offsetof(Node, link))));
            return *this;
        }

        const T& operator*() const { return *mPtr; }
        const T* operator->() const { return mPtr; }

        friend bool operator==(const constIterator& lhs, const constIterator& rhs) { return lhs.mPtr == rhs.mPtr; }
        friend bool operator!=(const constIterator& lhs, const constIterator& rhs) { return lhs.mPtr != rhs.mPtr; }

    private:
        const T* mPtr;
    };

    class robustIterator
    {
    public:
        robustIterator(T* ptr)
            : mPtr(ptr)
            , mNext(static_cast<ListNode*>(PtrUtil::addOffset(ptr, offsetof(Node, link)))->next())
        {
        }

        robustIterator& operator++()
        {
            mPtr = static_cast<T*>(PtrUtil::addOffset(mNext, -static_cast<intptr_t>(offsetof(Node, link))));
            mNext = mNext->next();
            return *this;
        }

        T& operator*() const { return *mPtr; }
        T* operator->() const { return mPtr; }

        friend bool operator==(const robustIterator& lhs, const robustIterator& rhs) { return lhs.mPtr == rhs.mPtr; }
        friend bool operator!=(const robustIterator& lhs, const robustIterator& rhs) { return lhs.mPtr != rhs.mPtr; }

    private:
        T* mPtr;
        ListNode* mNext;
    };

    class reverseIterator
    {
    public:
        reverseIterator(T* ptr)
            : mPtr(ptr)
        {
        }

        reverseIterator& operator++()
        {
            ListNode* prev = static_cast<ListNode*>(PtrUtil::addOffset(mPtr, offsetof(Node, link)))->prev();
            mPtr = static_cast<T*>(PtrUtil::addOffset(prev, -static_cast<intptr_t>(offsetof(Node, link))));
            return *this;
        }

        T& operator*() const { return *mPtr; }
        T* operator->() const { return mPtr; }

        friend bool operator==(const reverseIterator& lhs, const reverseIterator& rhs) { return lhs.mPtr == rhs.mPtr; }
        friend bool operator!=(const reverseIterator& lhs, const reverseIterator& rhs) { return lhs.mPtr != rhs.mPtr; }

    private:
        T* mPtr;

        friend class reverseConstIterator;
    };

    class reverseConstIterator
    {
    public:
        reverseConstIterator(const T* ptr)
            : mPtr(ptr)
        {
        }

        reverseConstIterator(const reverseIterator& it)
            : mPtr(it.mPtr)
        {
        }

        reverseConstIterator& operator++()
        {
            const ListNode* prev = static_cast<const ListNode*>(PtrUtil::addOffset(mPtr, offsetof(Node, link)))->prev();
            mPtr = static_cast<const T*>(PtrUtil::addOffset(prev, -static_cast<intptr_t>(offsetof(Node, link))));
            return *this;
        }

        const T& operator*() const { return *mPtr; }
        const T* operator->() const { return mPtr; }

        friend bool operator==(const reverseConstIterator& lhs, const reverseConstIterator& rhs) { return lhs.mPtr == rhs.mPtr; }
        friend bool operator!=(const reverseConstIterator& lhs, const reverseConstIterator& rhs) { return lhs.mPtr != rhs.mPtr; }

    private:
        const T* mPtr;
    };

    class reverseRobustIterator
    {
    public:
        reverseRobustIterator(T* ptr)
            : mPtr(ptr)
            , mPrev(static_cast<ListNode*>(PtrUtil::addOffset(ptr, offsetof(Node, link)))->prev())
        {
        }

        reverseRobustIterator& operator++()
        {
            mPtr = static_cast<T*>(PtrUtil::addOffset(mPrev, -static_cast<intptr_t>(offsetof(Node, link))));
            mPrev = mPrev->prev();
            return *this;
        }

        T& operator*() const { return *mPtr; }
        T* operator->() const { return mPtr; }

        friend bool operator==(const reverseRobustIterator& lhs, const reverseRobustIterator& rhs) { return lhs.mPtr == rhs.mPtr; }
        friend bool operator!=(const reverseRobustIterator& lhs, const reverseRobustIterator& rhs) { return lhs.mPtr != rhs.mPtr; }

    private:
        T* mPtr;
        ListNode* mPrev;
    };

public:
    iterator begin() { return iterator(listNodeToObj(mStartEnd.next())); }
    constIterator begin() const { return constIterator(listNodeToObj(mStartEnd.next())); }
    iterator end() { return iterator(listNodeToObj(&mStartEnd)); }
    constIterator end() const { return constIterator(listNodeToObj(&mStartEnd)); }

    constIterator constBegin() const { return constIterator(listNodeToObj(mStartEnd.next())); }
    constIterator constEnd() const { return constIterator(listNodeToObj(&mStartEnd)); }

    robustIterator robustBegin() { return robustIterator(listNodeToObj(mStartEnd.next())); }
    robustIterator robustEnd() { return robustIterator(listNodeToObj(&mStartEnd)); }

    reverseIterator reverseBegin() { return reverseIterator(listNodeToObj(mStartEnd.prev())); }
    reverseConstIterator reverseBegin() const { return reverseConstIterator(listNodeToObj(mStartEnd.prev())); }
    reverseIterator reverseEnd() { return reverseIterator(listNodeToObj(&mStartEnd)); }
    reverseConstIterator reverseEnd() const { return reverseConstIterator(listNodeToObj(&mStartEnd)); }

    reverseConstIterator reverseConstBegin() const { return reverseConstIterator(listNodeToObj(mStartEnd.prev())); }
    reverseConstIterator reverseConstEnd() const { return reverseConstIterator(listNodeToObj(&mStartEnd)); }

    reverseRobustIterator reverseRobustBegin() { return reverseRobustIterator(listNodeToObj(mStartEnd.prev())); }
    reverseRobustIterator reverseRobustEnd() { return reverseRobustIterator(listNodeToObj(&mStartEnd)); }

protected:
    static s32 compareT(const T* a, const T* b)
    {
        if (*a < *b)
        {
            return -1;
        }

        if (*b < *a)
        {
            return 1;
        }

        return 0;
    }

    ListNode* objToListNode(const T* obj) const
    {
        return static_cast<ListNode*>(PtrUtil::addOffset(obj, offsetof(Node, link)));
    }

    T* listNodeToObj(const ListNode* node) const
    {
        return static_cast<T*>(PtrUtil::addOffset(node, -static_cast<intptr_t>(offsetof(Node, link))));
    }

    T* listNodeToObjWithNullCheck(const ListNode* node) const
    {
        return node ? listNodeToObj(node) : nullptr;
    }

protected:
    FreeList mFreeList;
    s32 mLimitNum;
};

template <typename T, s32 N>
class FixedObjList : public ObjList<T>
{
public:
    FixedObjList()
        : ObjList<T>()
    {
        ObjList<T>::setBuffer(N, mInternalWork);
    }

    void allocBuffer(s32 limitNum, s32 alignment = cDefaultAlignment) = delete;
    void allocBuffer(s32 limitNum, Heap* heap, s32 alignment = cDefaultAlignment) = delete;
    bool tryAllocBuffer(s32 limitNum, s32 alignment = cDefaultAlignment) = delete;
    bool tryAllocBuffer(s32 limitNum, Heap* heap, s32 alignment = cDefaultAlignment) = delete;
    void freeBuffer() = delete;
    void setBuffer(s32 limitNum, void* buf) = delete;

protected:
    u8 mInternalWork[N * sizeof(typename ObjList<T>::Node)];
};

} // namespace sead
