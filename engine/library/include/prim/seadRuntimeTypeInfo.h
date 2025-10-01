#ifndef SEAD_RUNTIMETYPEINFO_H_
#define SEAD_RUNTIMETYPEINFO_H_

#include <basis/seadTypes.h>

namespace sead { namespace RuntimeTypeInfo {

struct Interface
{
    Interface() { }

    virtual bool isDerived(const Interface* other) const = 0;
};
#ifdef cafe
static_assert(sizeof(Interface) == 4, "sead::RuntimeTypeInfo::Interface size mismatch");
#endif // cafe

struct Root : public Interface
{
    Root() { }

    bool isDerived(const Interface* other) const override
    {
        return other == this;
    }
};

template <typename Super>
struct Derive : public Interface
{
    Derive() { }

    bool isDerived(const Interface* other) const override
    {
        if (this == other)
            return true;

        const RuntimeTypeInfo::Interface* rootTypeInfo = Super::getRuntimeTypeInfoStatic();
        return rootTypeInfo->isDerived(other);
    }
};

} // namespace sead::RuntimeTypeInfo

template <typename DerivedType, typename Type>
inline bool
IsDerivedTypes(const Type* ptr)
{
    const RuntimeTypeInfo::Interface* typeInfo = DerivedType::getRuntimeTypeInfoStatic();
    return ptr != nullptr && ptr->checkDerivedRuntimeTypeInfo(typeInfo);
}

template<typename DerivedType, typename Type>
inline DerivedType*
DynamicCast(Type* ptr)
{
    if (IsDerivedTypes<DerivedType, Type>(ptr))
        return static_cast<DerivedType*>(ptr);

    return nullptr;
}

template<typename DerivedType, typename Type>
inline const DerivedType*
DynamicCast(const Type* ptr)
{
    if (IsDerivedTypes<DerivedType, Type>(ptr))
        return static_cast<const DerivedType*>(ptr);

    return nullptr;
}

} // namespace sead

#define SEAD_RTTI_BASE(CLASS)                                                                                \
    public:                                                                                                  \
        static const sead::RuntimeTypeInfo::Interface* getRuntimeTypeInfoStatic();                           \
                                                                                                             \
        virtual bool checkDerivedRuntimeTypeInfo(const sead::RuntimeTypeInfo::Interface* type) const         \
        {                                                                                                    \
            const sead::RuntimeTypeInfo::Interface* clsTypeInfo = CLASS::getRuntimeTypeInfoStatic();         \
            return type == clsTypeInfo;                                                                      \
        }                                                                                                    \
                                                                                                             \
        virtual const sead::RuntimeTypeInfo::Interface* getRuntimeTypeInfo() const                           \
        {                                                                                                    \
            return getRuntimeTypeInfoStatic();                                                               \
        }

#define SEAD_RTTI_OVERRIDE(CLASS, BASE)                                                                      \
    public:                                                                                                  \
        static const sead::RuntimeTypeInfo::Interface* getRuntimeTypeInfoStatic();                           \
                                                                                                             \
        bool checkDerivedRuntimeTypeInfo(const sead::RuntimeTypeInfo::Interface* type) const override        \
        {                                                                                                    \
            const sead::RuntimeTypeInfo::Interface* clsTypeInfo = CLASS::getRuntimeTypeInfoStatic();         \
            if (type == clsTypeInfo)                                                                         \
                return true;                                                                                 \
                                                                                                             \
            return BASE::checkDerivedRuntimeTypeInfo(type);                                                  \
        }                                                                                                    \
                                                                                                             \
        const sead::RuntimeTypeInfo::Interface* getRuntimeTypeInfo() const override                          \
        {                                                                                                    \
            return getRuntimeTypeInfoStatic();                                                               \
        }

#define SEAD_RTTI_BASE_IMPL(CLASS)                                                                           \
    const sead::RuntimeTypeInfo::Interface* CLASS::getRuntimeTypeInfoStatic()                                \
    {                                                                                                        \
        static const sead::RuntimeTypeInfo::Root typeInfo;                                                   \
        return &typeInfo;                                                                                    \
    }                                                                                                        \

#define SEAD_RTTI_OVERRIDE_IMPL(CLASS, BASE)                                                                 \
    const sead::RuntimeTypeInfo::Interface* CLASS::getRuntimeTypeInfoStatic()                                \
    {                                                                                                        \
        static const sead::RuntimeTypeInfo::Derive<BASE> typeInfo;                                           \
        return &typeInfo;                                                                                    \
    }                                                                                                        \

#endif // SEAD_RUNTIMETYPEINFO_H_
