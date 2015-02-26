#include "ElementCollection.h"
#include "periodic.h"

#include <sifteo.h>

ElementCollection::ElementCollection()
{
    Clear();
}

void ElementCollection::Add(Element* element)
{
    // Don't add more than once:
    if (Contains(element))
    { return; }

    Assert(numUsed < capacity);
    elements[numUsed] = element;
    numUsed++;
}

Element* ElementCollection::Get(int index)
{
    Assert(index < numUsed);
    return elements[index];
}

void ElementCollection::Clear()
{
    PeriodicMemset(elements, 0, sizeof(elements));
    numUsed = 0;
}

int ElementCollection::Count()
{
    return numUsed;
}

int ElementCollection::Capacity()
{
    return capacity;
}

bool ElementCollection::Contains(Element* element)
{
    for (int i = 0; i < numUsed; i++)
    {
        if (elements[i] == element)
        { return true; }
    }

    return false;
}
