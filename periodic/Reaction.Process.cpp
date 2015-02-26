#include "Reaction.h"
#include "Element.h"

/*
Processes a reaction and determines the outcome, if any.

This algorithm was written with help by:
Christopher Culbertson, Associate Professor at Kansas State University
Michael Ayala, Chemistry Major at UC Davis
*/
bool Reaction::Process()
{
    Element* element;
    Element* other;
    ElementSet* elements;
    ElementSet* dedupe = new ElementSet();// Used to de-deupe two interactions with the same query
    ElementSet* others;

    //--------------------------------------------------------------------------
    // Determine all possible compounds: (2-element reactions)
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
    }
    delete elements;

    // Li - I
    elements = Find("Li");
    for (int i = 0; i < elements->Count(); i++)
    {
        element = elements->Get(i);
        if (element->GetBondWith("I", &other))
        { element->SetBondTypeFor(StartNewCompound(), other, BondType_Ionic); }
    }
    delete elements;

    //--------------------------------------------------------------------------
    // Determine all possible compounds: (3-element reactions)
    //--------------------------------------------------------------------------

    // AlkaliEarth - 2xHalogen/2xHydrogen
    elements = Find(ALKALIEARTH);
    for (int i = 0; i < elements->Count(); i++)
    {
        element = elements->Get(i);

        // Two halogens
        others = element->GetBondsWith(HALOGEN);
        if (others->Count() >= 2)
        {
            Compound* compound = StartNewCompound();
            //TODO: 2 and 9 are hard-coded as hacks to get to 4, 2, 2, need to make this more generic probably.
            element->SetBondTypeFor(compound, others->Get(0), BondType_Covalent, 2, 9);
            element->SetBondTypeFor(compound, others->Get(1), BondType_Covalent, 2, 9);
        }
        else if (others->Count() > 0)
        {
            // Potential reaction
            Compound* compound = StartNewCompound();
            for (int j = 0; j < others->Count(); j++)
            { element->SetBondTypeFor(compound, others->Get(j), BondType_Potential); }
        }
        delete others;

        // Two hydrogens
        others = element->GetBondsWith(HYDROGEN);
        if (others->Count() >= 2)
        {
            Compound* compound = StartNewCompound();

            //If the alkali earth metals are either Beryllium or Magnesium, the bond will be covalent
            //If they are the other alkali earth metals, they will make an ionic bond
            if (strcmp(element->GetSymbol(), "Be") == 0 || strcmp(element->GetSymbol(), "Mg") == 0)
            {
                //TODO: 6 and 9 are hard-coded as hacks to get to 4, 2, 2, need to make this more generic probably.
                element->SetBondTypeFor(compound, others->Get(0), BondType_Covalent, 3, 2);
                element->SetBondTypeFor(compound, others->Get(1), BondType_Covalent, 3, 2);
            }
            else
            {
                element->SetBondTypeFor(compound, others->Get(0), BondType_Ionic);
                element->SetBondTypeFor(compound, others->Get(1), BondType_Ionic);
            }
        }
        else if (others->Count() > 0)
        {
            // Potential reaction
            Compound* compound = StartNewCompound();
            for (int j = 0; j < others->Count(); j++)
            { element->SetBondTypeFor(compound, others->Get(j), BondType_Potential); }
        }
        delete others;
    }
    delete elements;

    //Cleanup
    delete dedupe;

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
        return true;
    }
    return false;
}
