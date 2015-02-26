#ifndef __COMPOUND_H__
#define __COMPOUND_H__

#include "Set.h"
#include "ObjectPool.h"
#include "ElementSet.h"
#include "periodic.h"

class Reaction;

class Compound : public ObjectPool<Compound, MAX_COMPOUNDS>
{
private:
    ElementSet elements;
    int index;
public:
    Compound();
    Compound(int index);
    void AddElement(Element* element);
    int GetElementCount();
    bool ContainsPotentialBonds();
    void Apply();
    int GetIndex();
};

#endif
