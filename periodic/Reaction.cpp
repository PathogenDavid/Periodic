#include "Reaction.h"
#include "Element.h"

Reaction::Reaction()
{
}

Reaction::~Reaction()
{
    for (int i = 0; i < possibleCompounds.Count(); i++)
    { delete possibleCompounds[i]; }
}

ElementSet* Reaction::Find(groupState group)
{
    ElementSet* ret = new ElementSet();
    for (int i = 0; i < elements.Count(); i++)
    {
        if (elements.Get(i)->GetGroup() == group)
        {
            ret->Add(elements.Get(i));
        }
    }
    return ret;
}

ElementSet* Reaction::Find(const char* symbol)
{
    ElementSet* ret = new ElementSet();
    for (int i = 0; i < elements.Count(); i++)
    {
        if (strcmp(elements.Get(i)->GetSymbol(), symbol) == 0)
        {
            ret->Add(elements.Get(i));
        }
    }
    return ret;
}

void Reaction::Add(Element* element)
{
    if (elements.Contains(element))
    { return; }

    elements.Add(element);
    element->SetReaction(this);
}

Compound* Reaction::StartNewCompound()
{
    // Create the new compound:
    Compound* ret = new Compound(possibleCompounds.Count());
    possibleCompounds.Add(ret);

    // Mark all elements in the reaction as not in use for this compound:
    ClearElementMasks();

    return ret;
}

void Reaction::CancelCompound(Compound* compound)
{
    possibleCompounds.Remove(compound);
    delete compound;
}

void Reaction::ClearElementMasks()
{
    for (int i = 0; i < elements.Count(); i++)
    { elements[i]->ClearMask(); }
}

void Reaction::ClearElementMasks(int bit)
{
    for (int i = 0; i < elements.Count(); i++)
    { elements[i]->ClearMaskBit(bit); }
}
