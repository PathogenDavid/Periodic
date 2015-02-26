#include "Compound.h"
#include "Reaction.h"
#include "Element.h"
#include "Bond.h"

Compound::Compound()
{}

Compound::Compound(int index)
{
    this->index = index;
    elements.Clear();
}

void Compound::AddElement(Element* element)
{
    elements.Add(element);
}

int Compound::GetElementCount()
{
    return elements.Count();
}

bool Compound::ContainsPotentialBonds()
{
    for (int i = 0; i < elements.Count(); i++)
    {
        for (int side = 0; side < BondSide_Count; side++)
        {
            if (elements[i]->GetBondTypeFor(this, (BondSide)side) == BondType_Potential)
            {
                return true;
            }
        }
    }

    return false;
}

int Compound::GetIndex()
{
    return index;
}

void Compound::Apply()
{
    for (int i = 0; i < elements.Count(); i++)
    {
        elements[i]->ApplyCompound(this);
    }
}
