#include "periodic.h"
#include "Element.h"
#include "Set.h"
#include "ObjectPool.h"

#include <sifteo.h>

class Reaction// : ObjectPool<Reaction, NUM_CUBES>
{
private:
    ElementSet elements;
    int nextCompoundIndex;
public:
    Reaction();
    ElementSet* Find(groupState group);
    ElementSet* Find(const char* symbol);
    void Add(Element* element);
    void Reset();

    void Process();
};
