#include "Reaction.h"
#include "Element.h"

class ReactionNode
{
protected:
    ReactionNode* firstChild = NULL;
    ReactionNode* sibling = NULL;
    BondType type = BondType_None;
    int leftData = 0;
    int rightData = 0;
public:
    class Iterator
    {
    private:
        ReactionNode** marker;
    public:
        Iterator(ReactionNode** start)
        {
            this->marker = start;
        }

        void Next()
        {
            if (*marker != NULL)
            { marker = &(*marker)->sibling; }
        }

        ReactionNode* Get() const
        {
            return *marker;
        }

        Iterator& operator++()
        {
            Next();
            return *this;
        }

        Iterator& operator++(int)
        {
            return ++(*this);
        }

        ReactionNode* operator*() const
        {
            return Get();
        }

        bool HasNext()
        {
            return *marker != NULL && (*marker)->sibling != NULL;
        }

        ReactionNode** GetMarker()
        {
            return marker;
        }

        void GoToEnd()
        {
            while (HasNext())
            { Next(); }
        }
    };

    void AddSibling(ReactionNode* node)
    {
        Iterator it(&sibling);
        it.GoToEnd();
        it.Next(); // Get marker on location to place node.
        *(it.GetMarker()) = node;
    }

    void AddChild(ReactionNode* node)
    {
        Iterator it(&firstChild);
        it.GoToEnd();
        it.Next(); // Get marker on location to place node.
        *(it.GetMarker()) = node;
    }

    void SetBondInfo(BondType type, int leftData, int rightData)
    {
        this->type = type;
        this->leftData = leftData;
        this->rightData = rightData;
    }

    void SetBondInfo(BondType type, int data)
    { SetBondInfo(type, data, data); }

    void SetBondInfo(BondType type)
    { SetBondInfo(type, 0, 0); }

    void ApplyBond(Compound* compound, Element* left, Element* right)
    {
        if (this->type == BondType_None)
        { return; }

        LOG("ApplyBond(Compound:0x%X, Element:0x%X, Element:0x%X)\n", compound, left, right);
        Assert(left != right); // This means a bond was applied to a passthrough node.

        left->SetBondTypeFor(compound, right, type, leftData, rightData);
    }

    bool ProcessChildren(Compound* compound, Element* inputForChildren)
    {
        LOG("Processing children for 0x%X\n", this);
        for (Iterator it(&firstChild); *it; it++)
        {
            LOG("Child for 0x%X = 0x%X\n", this, *it);
            if (!(*it)->Process(compound, inputForChildren))
            {
                LOG("Child for 0x%X failed to process.\n", this);
                return false; // All children must return true to succeed.
            }
        }
        LOG("Done processing children for 0x%X\n", this);

        return true; // If we got this far, all children met their criteria.
    }

    // Don't override this for most implementations of this class
    virtual bool Process(Compound* compound, Element* input)
    {
        Element* output = GetOutput(compound, input);

        if (output == NULL)
        { return false; }

        output->SetMaskBit(1); // Mark this element as being used

        if (!ProcessChildren(compound, output))
        {
            output->ClearMaskBit(1); // Free this element since we no longer need it
            return false;
        }

        ApplyBond(compound, input, output);
        return true;
    }

    // Override this for most implementations of this class
    virtual Element* GetOutput(Compound* compound, Element* input)
    {
        return NULL;
    }
};

class RootElementSymbolNode : public ReactionNode
{
private:
    const char* symbol;
public:
    RootElementSymbolNode(const char* symbol)
    {
        this->symbol = symbol;
        LOG("RootElementSymbolNode:0x%X %s\n", this, symbol);
    }

    virtual Element* GetOutput(Compound* compound, Element* input)
    {
        return strcmp(input->GetSymbol(), symbol) == 0 ? input : NULL;
    }
};

class ElementSymbolNode : public ReactionNode
{
private:
    const char* symbol;
public:
    ElementSymbolNode(const char* symbol)
    {
        this->symbol = symbol;
        LOG("ElementSymbolNode:0x%X %s\n", this, symbol);
    }

    virtual Element* GetOutput(Compound* compound, Element* input)
    {
        Element* ret = input->GetBondWith(symbol);
        LOG("GetOutput for %s = 0x%X\n", symbol, ret);
        return ret;
    }
};

/*
Processes a reaction and determines the outcome, if any.

This algorithm was written with help by:
Christopher Culbertson, Associate Professor at Kansas State University
Michael Ayala, Chemistry Major at UC Davis
*/
bool Reaction::Process()
{
    LOG("--------------------------------------------------------------\n");
    //ElementSymbolNode node1("H");
    //ElementSymbolNode node2("H");
    //node1.AddChild(&node2);
    //node2.SetBondInfo(BondType_Covalent);
    RootElementSymbolNode node1("Cl");
    ElementSymbolNode node1_1("O");
    ElementSymbolNode node1_1_1("H");
    ElementSymbolNode node1_2("O");
    ElementSymbolNode node1_3("O");
    ElementSymbolNode node1_4("O");

    node1.AddChild(&node1_1);
    node1.AddChild(&node1_2);
    node1.AddChild(&node1_3);
    node1.AddChild(&node1_4);

    node1_1.AddChild(&node1_1_1);

    node1_1.SetBondInfo(BondType_Covalent, 1);
    node1_2.SetBondInfo(BondType_Covalent, 2);
    node1_3.SetBondInfo(BondType_Covalent, 2);
    node1_4.SetBondInfo(BondType_Covalent, 2);

    node1_1_1.SetBondInfo(BondType_Covalent, 1);

    //TODO: Currently, it is theoretically possible for sufficiently complex compounds to find invalid reactions in some case.
    /*
    We should add some processing to make sure we don't re-use elements in the same reaction.

    For instance, say the user is trying to make phsophorous acid (H3PO3) the proper cube arrangement would be something like
      OH
    HOP
      OH
    However, it is possible that this could happen:
      OH
    HOPO
    Where the right-top hydrogen is shared between the two oxygens, which is not physically possible.
    */

    Compound* newCompound = StartNewCompound();
    for (int i = 0; i < elements.Count(); i++)
    {
        LOG("Processing element %d:%s\n", i, elements[i]->GetSymbol());
        if (node1.Process(newCompound, elements[i]))
        {
            LOG("%d: TRUE\n", i);
            newCompound = StartNewCompound();
        }
        else
        {
            LOG("%d: FALSE\n", i);
        }
    }
    CancelCompound(newCompound);

    LOG("%d!!!\n", possibleCompounds.Count());

    #if 0
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
        { element->SetBondTypeFor(StartNewCompound(), other, BondType_Ionic);}  
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
            //If the alkali earth metal is Beryllium, the bond will be covalent
            //If they are the other alkali earth metals, they will make an ionic bond
            //TODO: 2 and 9 are hard-coded as hacks to get to 4, 2, 2, need to make this more generic probably.
            if (strcmp(element->GetSymbol(), "Be") == 0)
            {
                element->SetBondTypeFor(compound, others->Get(0), BondType_Covalent, 2, 9);
                element->SetBondTypeFor(compound, others->Get(1), BondType_Covalent, 2, 9);
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
    #endif

    //--------------------------------------------------------------------------
    // Choose the ideal compound and apply it:
    //--------------------------------------------------------------------------
    //TODO: Allow multiple compounds to form in one reaction when they don't overlap.
    idealCompound = NULL;
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

    // Dispose of all compounds other than the ideal one to save memory
    for (int i = 0; i < possibleCompounds.Count(); i++)
    {
        if (possibleCompounds[i] != idealCompound)
        {
            delete possibleCompounds[i];
        }
    }
    // Clear the possible compounds list to release the linked list memory (The ideal compound will survive.)
    possibleCompounds.Clear();

    // Apply the ideal compound and return
    if (idealCompound != NULL)
    {
        idealCompound->Apply();
        return true;
    }
    return false;
}
