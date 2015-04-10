#include "periodic.h"
#include "Element.h"
#include "Set.h"
#include "Compound.h"
#include "ObjectPool.h"
#include "LinkedList.h"

#include <sifteo.h>

class Trie;

class Reaction : public ObjectPool<Reaction, MAX_REACTIONS>
{
private:
    ElementSet elements;
    LinkedList<Compound*, MAX_COMPOUNDS> possibleCompounds;
    Compound* idealCompound = NULL;
public:
    Reaction();
    ~Reaction();
    ElementSet* Find(groupState group);
    ElementSet* Find(const char* symbol);
    void Add(Element* element);

    bool Process();
private:
    Compound* StartNewCompound();
    void CancelCompound(Compound* compound);
    
    void ClearElementMasks();
    void ClearElementMasks(int bit);
};
