#ifndef __OBJECTPOOL_H__
#define __OBJECTPOOL_H__

#include <sifteo.h>
#include "periodic.h"

template<class PoolT, int PoolSizeT>
class ObjectPool
{
private:
    static PoolT objectPool[PoolSizeT];
    static int numUninitialized;

    bool inUse;
public:
    ObjectPool()
    {
        // We need to initialize this only once, otherwise we end up resetting the value every time a new instance is instantiated.
        if (numUninitialized > 0)
        {
            numUninitialized--;
            inUse = false;
        }
    }

    static void* operator new(size_t size)
    {
        Assert(size == sizeof(PoolT)); // Something has gone very wrong

        // Look for a free object in the pool and return it
        for (int i = 0; i < PoolSizeT; i++)
        {
            if (objectPool[i].inUse) { continue; }
            objectPool[i].inUse = true;
            return &objectPool[i];
        }

        // If we get this far, we've exhausted the object pool
        LOG("FATAL: Object pool with %d elements exhausted!", PoolSizeT);
        AssertAlways();
        return NULL;
    }

    static void operator delete(void* p)
    {
        ((PoolT*)p)->inUse = false;
    }
};

template<class PoolT, int PoolSizeT>
PoolT ObjectPool<PoolT, PoolSizeT>::objectPool[PoolSizeT]; // Note: Be aware that this implicitly calls the default constructor for PoolT.

template<class PoolT, int PoolSizeT>
int ObjectPool<PoolT, PoolSizeT>::numUninitialized = PoolSizeT;

#endif
