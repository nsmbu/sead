#ifndef SEAD_TLIST_H_
#define SEAD_TLIST_H_

#include <container/seadListImpl.h>

namespace sead {

template <typename T>
class TList;

template <typename T>
class TListNode : public ListNode
{
public:
    TListNode()
        : ListNode()
        , mData()
        , mList(nullptr)
    {
    }

    TListNode(T data)
        : ListNode()
        , mData(data)
        , mList(nullptr)
    {
    }

    void insertBack(TListNode<T>* n);
    void insertFront(TListNode<T>* n);

    void erase()
    {
        if (mList != nullptr)
            mList->erase(this);
    }

    T mData;
    TList<T>* mList;

    friend class TList<T>;
};
#ifdef cafe
static_assert(sizeof(TListNode<int*>) == 0x10, "sead::TListNode<T> size mismatch");
#endif // cafe

template <typename T>
class TList : public ListImpl
{
private:
    using CompareCallback = s32 (*)(const T* a, const T* b);

public:
    TList()
        : ListImpl()
    {
    }

    void pushBack(TListNode<T>* obj)
    {
        if (obj->mList != nullptr)
            obj->mList->erase(obj);

        obj->mList = this;
        ListImpl::pushBack(obj);
    }

    void pushFront(TListNode<T>* obj);
    TListNode<T>* popBack();
    TListNode<T>* popFront();
    void insertBefore(TListNode<T>* basis, TListNode<T>* obj);
    void insertAfter(TListNode<T>* basis, TListNode<T>* obj);

    void erase(TListNode<T>* obj)
    {
        if (obj->mList != nullptr)
        {
            obj->mList = nullptr;
            ListImpl::erase(obj);
        }
    }

    TListNode<T>* front() const;
    TListNode<T>* back() const;
    TListNode<T>* prev(const TListNode<T>* obj) const;
    TListNode<T>* next(const TListNode<T>* obj) const;

    TListNode<T>* nth(s32 index) const
    {
        return static_cast<TListNode<T>*>(ListImpl::nth(index));
    }

    s32 indexOf(const TListNode<T>* obj) const;
    bool isNodeLinked(const TListNode<T>* obj) const;
    void swap(TListNode<T>* obj1, TListNode<T>* obj2);
    void moveAfter(TListNode<T>* basis, TListNode<T>* obj);
    void moveBefore(TListNode<T>* basis, TListNode<T>* obj);

    void clear()
    {
        TListNode<T>* n = static_cast<TListNode<T>*>(mStartEnd.next());
        while (n != &mStartEnd)
        {
            TListNode<T>* next = static_cast<TListNode<T>*>(n->next());
            erase(n);
            n = next;
        }
    }

    void unsafeClear() { ListImpl::unsafeClear(); }

    void sort() { sort(&compareT); }

    void sort(CompareCallback cmp)
    {
        ListImpl::sort(-static_cast<s32>(offsetof(TListNode<T>, mData)), reinterpret_cast<CompareCallbackImpl>(cmp));
    }

    void mergeSort() { mergeSort(&compareT); }

    void mergeSort(CompareCallback cmp)
    {
        ListImpl::mergeSort(-static_cast<s32>(offsetof(TListNode<T>, mData)), reinterpret_cast<CompareCallbackImpl>(cmp));
    }

    TListNode<T>* find(const T* obj) const { return find(obj, &compareT); }

    TListNode<T>* find(const T* obj, CompareCallback cmp) const
    {
        return static_cast<TListNode<T>*>(ListImpl::find(obj, -static_cast<s32>(offsetof(TListNode<T>, mData)), reinterpret_cast<CompareCallbackImpl>(cmp)));
    }

    void uniq() { uniq(&compareT); }

    void uniq(CompareCallback cmp)
    {
        ListImpl::uniq(-static_cast<s32>(offsetof(TListNode<T>, mData)), reinterpret_cast<CompareCallbackImpl>(cmp));
    }

public:
    class iterator
    {
    public:
        explicit iterator(TListNode<T>* ptr)
            : mPtr(ptr)
        {
        }

        iterator& operator++()
        {
            mPtr = static_cast<TListNode<T>*>(mPtr->next());
            return *this;
        }

    public:
        T& operator*() const
        {
            return mPtr->mData;
        }

        T* operator->() const
        {
            return &mPtr->mData;
        }

        friend bool operator==(const iterator& it1, const iterator& it2)
        {
            return it1.mPtr == it2.mPtr;
        }

        friend bool operator!=(const iterator& it1, const iterator& it2)
        {
            return it1.mPtr != it2.mPtr;
        }

    protected:
        TListNode<T>* mPtr;

        friend class constIterator;
    };

    class constIterator
    {
    public:
        explicit constIterator(const TListNode<T>* ptr)
            : mPtr(ptr)
        {
        }

        constIterator(const iterator& it)
            : mPtr(it.mPtr)
        {
        }

        constIterator& operator++()
        {
            mPtr = static_cast<const TListNode<T>*>(mPtr->next());
            return *this;
        }

    public:
        const T& operator*() const
        {
            return mPtr->mData;
        }

        const T* operator->() const
        {
            return &mPtr->mData;
        }

        friend bool operator==(const constIterator& it1, const constIterator& it2)
        {
            return it1.mPtr == it2.mPtr;
        }

        friend bool operator!=(const constIterator& it1, const constIterator& it2)
        {
            return it1.mPtr != it2.mPtr;
        }

    protected:
        const TListNode<T>* mPtr;
    };

    class robustIterator
    {
    public:
        explicit robustIterator(TListNode<T>* ptr)
            : mPtr(ptr)
            , mNext(static_cast<TListNode<T>*>(ptr->next()))
        {
        }

        robustIterator& operator++()
        {
            mPtr = mNext;
            mNext = static_cast<TListNode<T>*>(mNext->next());
            return *this;
        }

    public:
        TListNode<T>& operator*() const
        {
            return *mPtr;
        }

        TListNode<T>* operator->() const
        {
            return mPtr;
        }

        friend bool operator==(const robustIterator& it1, const robustIterator& it2)
        {
            return it1.mPtr == it2.mPtr;
        }

        friend bool operator!=(const robustIterator& it1, const robustIterator& it2)
        {
            return it1.mPtr != it2.mPtr;
        }

    protected:
        TListNode<T>* mPtr;
        TListNode<T>* mNext;
    };

    class reverseIterator
    {
    public:
        explicit reverseIterator(TListNode<T>* ptr)
            : mPtr(ptr)
        {
        }

        reverseIterator& operator++()
        {
            mPtr = static_cast<TListNode<T>*>(mPtr->prev());
            return *this;
        }

        T& operator*() const { return mPtr->mData; }
        T* operator->() const { return &mPtr->mData; }

        friend bool operator==(const reverseIterator& lhs, const reverseIterator& rhs) { return lhs.mPtr == rhs.mPtr; }
        friend bool operator!=(const reverseIterator& lhs, const reverseIterator& rhs) { return lhs.mPtr != rhs.mPtr; }

    protected:
        TListNode<T>* mPtr;

        friend class reverseConstIterator;
    };

    class reverseConstIterator
    {
    public:
        explicit reverseConstIterator(const TListNode<T>* ptr)
            : mPtr(ptr)
        {
        }

        reverseConstIterator(const reverseIterator& it)
            : mPtr(it.mPtr)
        {
        }

        reverseConstIterator& operator++()
        {
            mPtr = static_cast<const TListNode<T>*>(mPtr->prev());
            return *this;
        }

        const T& operator*() const { return mPtr->mData; }
        const T* operator->() const { return &mPtr->mData; }

        friend bool operator==(const reverseConstIterator& lhs, const reverseConstIterator& rhs) { return lhs.mPtr == rhs.mPtr; }
        friend bool operator!=(const reverseConstIterator& lhs, const reverseConstIterator& rhs) { return lhs.mPtr != rhs.mPtr; }

    protected:
        const TListNode<T>* mPtr;
    };

    class reverseRobustIterator
    {
    public:
        explicit reverseRobustIterator(TListNode<T>* ptr)
            : mPtr(ptr)
            , mPrev(static_cast<TListNode<T>*>(ptr->prev()))
        {
        }

        reverseRobustIterator& operator++()
        {
            mPtr = mPrev;
            mPrev = static_cast<TListNode<T>*>(mPrev->prev());
            return *this;
        }

        TListNode<T>& operator*() const { return *mPtr; }
        TListNode<T>* operator->() const { return mPtr; }

        friend bool operator==(const reverseRobustIterator& lhs, const reverseRobustIterator& rhs) { return lhs.mPtr == rhs.mPtr; }
        friend bool operator!=(const reverseRobustIterator& lhs, const reverseRobustIterator& rhs) { return lhs.mPtr != rhs.mPtr; }

    protected:
        TListNode<T>* mPtr;
        TListNode<T>* mPrev;
    };

public:
    iterator begin() const { return iterator(static_cast<TListNode<T>*>(mStartEnd.next())); }
    iterator end() const { return iterator(static_cast<TListNode<T>*>(const_cast<ListNode*>(&mStartEnd))); }
    iterator toIterator(TListNode<T>* obj) const { return iterator(obj); }

    constIterator constBegin() const { return constIterator(static_cast<TListNode<T>*>(mStartEnd.next())); }
    constIterator constEnd() const { return constIterator(static_cast<TListNode<T>*>(const_cast<ListNode*>(&mStartEnd))); }
    constIterator toConstIterator(const TListNode<T>* obj) const { return constIterator(obj); }

    robustIterator robustBegin() const { return robustIterator(static_cast<TListNode<T>*>(mStartEnd.next())); }
    robustIterator robustEnd() const { return robustIterator(static_cast<TListNode<T>*>(const_cast<ListNode*>(&mStartEnd))); }
    robustIterator toRobustIterator(TListNode<T>* obj) const { return robustIterator(obj); }

    reverseIterator reverseBegin() const { return reverseIterator(static_cast<TListNode<T>*>(mStartEnd.prev())); }
    reverseIterator reverseEnd() const { return reverseIterator(static_cast<TListNode<T>*>(const_cast<ListNode*>(&mStartEnd))); }
    reverseIterator toReverseIterator(TListNode<T>* obj) const { return reverseIterator(obj); }

    reverseConstIterator reverseConstBegin() const { return reverseConstIterator(static_cast<TListNode<T>*>(mStartEnd.prev())); }
    reverseConstIterator reverseConstEnd() const { return reverseConstIterator(static_cast<TListNode<T>*>(const_cast<ListNode*>(&mStartEnd))); }
    reverseConstIterator toReverseConstIterator(const TListNode<T>* obj) const { return reverseConstIterator(obj); }

    reverseRobustIterator reverseRobustBegin() const { return reverseRobustIterator(static_cast<TListNode<T>*>(mStartEnd.prev())); }
    reverseRobustIterator reverseRobustEnd() const { return reverseRobustIterator(static_cast<TListNode<T>*>(const_cast<ListNode*>(&mStartEnd))); }
    reverseRobustIterator toReverseRobustIterator(TListNode<T>* obj) const { return reverseRobustIterator(obj); }

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
};

} // namespace sead

#endif // SEAD_TLIST_H_
