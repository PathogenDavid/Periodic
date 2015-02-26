#include "Reaction.h"
#include "Element.h"

void Reaction::Process()
{
    Element* element;
    Element* other;
    ElementSet* elements;
    ElementSet* dedupe;// Used to de-deupe two interactions with the same query

    //--------------------------------------------------------------------------
    // Determine all possible compounds:
    //--------------------------------------------------------------------------

    // Hydrogen - Hydrogen/Halogen/Alkali/AlkaliEarth
    elements = Find(HYDROGEN);
    dedupe = new ElementSet();
    for (int i = 0; i < elements->Count(); i++)
    {
        element = elements->Get(i);

        if (!dedupe->Contains(element) && (other = element->GetBondWith(HYDROGEN)))
        {
            dedupe->Add(other);
            element->SetBondTypeFor(StartNewCompound(), other, BondType_Covalent, 2, 2);
        }

        if (other = element->GetBondWith(HALOGEN))
        { element->SetBondTypeFor(StartNewCompound(), other, BondType_Covalent, 2, 8); }

        if (other = element->GetBondWith(ALKALI))
        { element->SetBondTypeFor(StartNewCompound(), other, BondType_Covalent, 2, 2); }

        if (other = element->GetBondWith(ALKALIEARTH))
        { element->SetBondTypeFor(StartNewCompound(), other, BondType_Potential); }
    }
    delete elements;

    //--------------------------------------------------------------------------
    // Choose the ideal compound:
    //--------------------------------------------------------------------------
    //TODO: Allow multiple compounds to form in one reaction.
}
