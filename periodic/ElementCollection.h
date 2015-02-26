#ifndef __ELEMENTCOLLECTION_H__
#define __ELEMENTCOLLECTION_H__

#include "ObjectPool.h"

class Element;

class ElementCollection : public ObjectPool<ElementCollection, 10>
{
private:
    Element* elements[NUM_CUBES];
    int numUsed = 0;
    const int capacity = NUM_CUBES;
public:
    ElementCollection();
    void Add(Element* element);
    Element* Get(int index);
    void Clear();
    int Count();
    int Capacity();
    bool Contains(Element* element);
};

#endif
