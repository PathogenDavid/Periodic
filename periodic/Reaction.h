#include "periodic.h"
#include "Element.h"
#include "Set.h"
#include "Compound.h"
#include "ObjectPool.h"

#include <sifteo.h>

class Trie;

class Reaction : public ObjectPool<Reaction, MAX_REACTIONS>
{
private:
    ElementSet elements;
    Set<Compound, MAX_COMPOUNDS> possibleCompounds;
public:
    Reaction();
    ~Reaction();
    ElementSet* Find(groupState group);
    ElementSet* Find(const char* symbol);
    void Add(Element* element);

    bool Process();
private:
    Compound* StartNewCompound();

    static bool trieIsInitialized;
    static Trie trie;
    static void InitializeTrie();
};
