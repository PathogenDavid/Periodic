#include "Reaction.h"
#include "Element.h"

Reaction::Reaction()
{
}

Reaction::~Reaction()
{
    Assert(possibleCompounds.Count() == 0); // The possible compounds list should never have contents outside of Reaction::Process.
    delete idealCompound; // Delete the ideal compound that is in use in this reaction if we have one.
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
    //TODO: Right now this function is actually really dangerous because a lot of the bond stuff relies on the indices associated with each compound being consistent.
    //      We need a way for the compound to purge its self from all Elements' memories. However, there is still the issue of deleting a compound not at the end of the list.
    Assert(compound->GetIndex() == (possibleCompounds.Count() - 1)); // Ensure that this cancelation is at least a little safe-ish.
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
