#include "periodic.h"
#include "Element.h"
#include "Set.h"
#include "Compound.h"

#include <sifteo.h>

class Reaction
{
private:
    ElementSet elements;
    Set<Compound, MAX_COMPOUNDS> possibleCompounds;
public:
    Reaction();
    ElementSet* Find(groupState group);
    ElementSet* Find(const char* symbol);
    void Add(Element* element);
    void Reset();

    bool Process();
private:
    Compound* StartNewCompound();
};
