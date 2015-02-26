#ifndef __SET_H__
#define __SET_H__

#include "ObjectPool.h"

template<class T, int CapacityT>
class Set : public ObjectPool<Set<T, CapacityT>, 10>
{
private:
    T* items[CapacityT];
    int numUsed = 0;
public:
    void Clear()
    {
        PeriodicMemset(items, 0, sizeof(items));
        numUsed = 0;
    }
    
    Set()
    {
        Clear();
    }

    void Add(T* item)
    {
        // Don't add more than once:
        if (Contains(item))
        {
            return;
        }

        Assert(numUsed < CapacityT);
        items[numUsed] = item;
        numUsed++;
    }

    int IndexOf(T* item)
    {
        for (int i = 0; i < numUsed; i++)
        {
            if (items[i] == item)
            {
                return i;
            }
        }

        return -1;
    }

    T* Get(int index)
    {
        Assert(index >= 0 && index < numUsed);
        return items[index];
    }

    void SetNull(int index)
    {
        Assert(index >= 0 && index < numUsed);
        items[index] = NULL;
    }

    void SetNull(T* item)
    {
        SetNull(IndexOf(item));
    }

    T* operator[](int index)
    {
        return Get(index);
    }

    int Count()
    {
        return numUsed;
    }

    int Capacity()
    {
        return CapacityT;
    }

    bool Contains(T* item)
    {
        for (int i = 0; i < numUsed; i++)
        {
            if (items[i] == item)
            { return true; }
        }

        return false;
    }
};

#endif
