#include "Reaction.h"
#include "ElementCollection.h"

Reaction::Reaction()
{
}

ElementCollection* Reaction::Find(groupState group)
{
    ElementCollection* ret = new ElementCollection();
    for (int i = 0; i < elements.Count(); i++)
    {
        if (elements.Get(i)->GetGroup() == group)
        {
            ret->Add(elements.Get(i));
        }
    }
    return ret;
}

ElementCollection* Reaction::Find(const char* symbol)
{
    ElementCollection* ret = new ElementCollection();
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
    elements.Add(element);
    element->SetReaction(this);
}

void Reaction::Reset()
{
    elements.Clear();
}

void Reaction::Process()
{
    // Determine all possible compounds:

    // Choose the ideal compound:
}

