#include "Reaction.h"
#include "Element.h"

Reaction::Reaction()
{
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

void Reaction::Reset()
{
    elements.Clear();
    
    for (int i = 0; i < possibleCompounds.Count(); i++)
    { delete possibleCompounds[i]; }
    possibleCompounds.Clear();
}

Compound* Reaction::StartNewCompound()
{
    Compound* ret = new Compound(possibleCompounds.Count());
    possibleCompounds.Add(ret);
    return ret;
}
