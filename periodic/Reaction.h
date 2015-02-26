#include "periodic.h"
#include "Element.h"
#include "ElementCollection.h"
#include "ObjectPool.h"

#include <sifteo.h>

class Reaction// : ObjectPool<Reaction, NUM_CUBES>
{
private:
    ElementCollection elements;
public:
    Reaction();
    ElementCollection* Find(groupState group);
    ElementCollection* Find(const char* symbol);
    void Add(Element* element);
    void Reset();

    void Process();
};
