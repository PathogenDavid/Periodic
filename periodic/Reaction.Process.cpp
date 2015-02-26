#include "Reaction.h"
#include "Element.h"

void Reaction::Process()
{
    Element* element;
    Element* other;
    ElementSet* elements;
    ElementSet* dedupe = new ElementSet();// Used to de-deupe two interactions with the same query

    //--------------------------------------------------------------------------
    // Determine all possible compounds:
    //--------------------------------------------------------------------------

    // Hydrogen - Hydrogen/Halogen/Alkali/AlkaliEarth
    elements = Find(HYDROGEN);
    dedupe->Clear();
    for (int i = 0; i < elements->Count(); i++)
    {
        element = elements->Get(i);

        if (!dedupe->Contains(element) && element->GetBondWith(HYDROGEN, &other))
        {
            dedupe->Add(other);
            element->SetBondTypeFor(StartNewCompound(), other, BondType_Covalent, 2, 2);
        }

        if (element->GetBondWith(HALOGEN, &other))
        { element->SetBondTypeFor(StartNewCompound(), other, BondType_Covalent, 2, 8); }

        if (element->GetBondWith(ALKALI, &other))
        { element->SetBondTypeFor(StartNewCompound(), other, BondType_Covalent, 2, 2); }

        if (element->GetBondWith(ALKALIEARTH, &other))
        { element->SetBondTypeFor(StartNewCompound(), other, BondType_Potential); }
    }
    delete elements;

    // Halogen - Halogen/Alakali/AlakaliEarth
    elements = Find(HALOGEN);
    dedupe->Clear();
    for (int i = 0; i < elements->Count(); i++)
    {
        element = elements->Get(i);

        if (!dedupe->Contains(element) && element->GetBondWith(HALOGEN, &other))
        {
            dedupe->Add(other);
            element->SetBondTypeFor(StartNewCompound(), other, BondType_Covalent, 8, 8);
        }

        if (element->GetBondWith(ALKALI, &other))
        { element->SetBondTypeFor(StartNewCompound(), other, BondType_Ionic); }

        if (element->GetBondWith(ALKALIEARTH, &other))
        { element->SetBondTypeFor(StartNewCompound(), other, BondType_Potential); }
    }
    delete elements;

    // Li - I
    elements = Find("Li");
    for (int i = 0; i < elements->Count(); i++)
    {
        element = elements->Get(i);
        if (element->GetBondWith("I"))
        { element->SetBondTypeFor(StartNewCompound(), other, BondType_Ionic); }
    }
    delete elements;

    //--------------------------------------------------------------------------
    // Choose the ideal compound and apply it:
    //--------------------------------------------------------------------------
    //TODO: Allow multiple compounds to form in one reaction when they don't overlap.
    Compound* idealCompound = NULL;
    for (int i = 0; i < possibleCompounds.Count(); i++)
    {
        Compound* compound = possibleCompounds[i];

        if (idealCompound == NULL)
        {
            idealCompound = compound;
            continue;
        }

        if (idealCompound->ContainsPotentialBonds() && !compound->ContainsPotentialBonds())
        {
            idealCompound = compound;
            continue;
        }

        if (idealCompound->GetElementCount() < compound->GetElementCount())
        {
            idealCompound = compound;
            continue;
        }
    }

    if (idealCompound != NULL)
    {
        idealCompound->Apply();
    }
}
