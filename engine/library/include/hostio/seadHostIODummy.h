#ifndef SEAD_HOST_IO_DUMMY_H_
#define SEAD_HOST_IO_DUMMY_H_

#include <basis/seadTypes.h>

namespace sead { namespace hostio {

class LifeCheckable
{
};

class PropertyEventListener : public LifeCheckable
{
};

class NodeEventListener : public PropertyEventListener
{
};

class Reflexible : public NodeEventListener
{
};

class Node : public Reflexible
{
};

class Context
{
    class ContextBufferAccessor
    {
    };
};

} } // namespace sead::hostio

#endif // SEAD_HOST_IO_DUMMY_H_
