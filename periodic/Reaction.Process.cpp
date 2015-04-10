#include "Reaction.h"
#include "Element.h"

#define MAX_REACTION_DEPTH (sizeof(uint32) * 8 - 1)
#define ELEMENT_IN_USE_BIT (MAX_REACTION_DEPTH + 1)

//HACK: This is to get around ReactionNode not having a reference to the current reaction, make this not awful later.
Reaction* currentReaction;

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

        uint32 Count()
        {
            uint32 ret = 0;
            while (HasNext())
            {
                Next();
                ret++;
            }
            return ret;
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

        //LOG("ApplyBond(Compound:0x%X, Element:0x%X, Element:0x%X)\n", compound, left, right);
        Assert(left != right); // This means a bond was applied to a passthrough node.

        left->SetMaskBit(MAX_REACTION_DEPTH);
        right->SetMaskBit(MAX_REACTION_DEPTH);
        left->SetBondTypeFor(compound, right, type, leftData, rightData);
    }

    bool ProcessChildren(Compound* compound, Element* inputForChildren, int depth)
    {
        for (Iterator it(&firstChild); *it; it++)
        {
            if (!(*it)->Process(compound, inputForChildren, depth))
            {
                return false; // All children must return true to succeed.
            }
        }

        return true; // If we got this far, all children met their criteria.
    }

    // Don't override this for most implementations of this class
    virtual bool Process(Compound* compound, Element* input, int depth)
    {
        Assert(depth >= 0 && depth < MAX_REACTION_DEPTH);
        bool success = false;
        Element* output = NULL;

        while (true)
        {
            // This will return a different element each time it is called, and NULL when no element is available
            // Therefore, we can loop until we find an element with child elements that satisfy this branch of the reaction.
            output = GetOutput(compound, input, depth);

            // If output == NULL, we ran out of possibilities and failed.
            if (output == NULL)
            { break; }

            // Mark this element as being used for this depth of the compound processing
            output->SetMaskBit(depth);

            // Process children (on the next depth)
            if (ProcessChildren(compound, output, depth + 1))
            {
                // All children succeeded, so we succeed!
                success = true;
                break;
            }
        }

        currentReaction->ClearElementMasks(depth); // Clear all of the elements we considered for this branch
        if (success)
        { ApplyBond(compound, input, output); }
        return success;
    }

    // Override this for most implementations of this class
    virtual Element* GetOutput(Compound* compound, Element* input, int depth)
    {
        LOG("WARN: Called unimplemented ReactionNode:GetOutput with Element:0x%X[%s] as input @ depth %d!\n", input, input->GetSymbol(), depth);
        return NULL;
    }

    Iterator GetChildIterator()
    {
        return Iterator(&firstChild);
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

    virtual Element* GetOutput(Compound* compound, Element* input, int depth)
    {
        if (input->MatchesMask(ALL_ELEMENTS_MASK))
        { return NULL; }

        Element* ret = strcmp(input->GetSymbol(), symbol) == 0 ? input : NULL;
        //LOG("Node:0x%X[%s].GetOutput(__, __, %d) = 0x%X\n", this, symbol, depth, ret);
        return ret;
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

    virtual Element* GetOutput(Compound* compound, Element* input, int depth)
    {
        Element* ret = input->GetBondWith(symbol);
        //LOG("Node:0x%X[%s].GetOutput(__, __, %d) = 0x%X\n", this, symbol, depth, ret);
        return ret;
    }
};

void InitializeCompoundDatabase();

ReactionNode CompoundDatabaseRoot;

/*
Processes a reaction and determines the outcome, if any.

This algorithm was written with help by:
Christopher Culbertson, Associate Professor at Kansas State University
Michael Ayala, Chemistry Major at UC Davis
*/
bool Reaction::Process()
{
    // Initialize the compound database
    static bool compoundDatabaseIsInitialized = false;
    if (!compoundDatabaseIsInitialized)
    {
        InitializeCompoundDatabase();
        compoundDatabaseIsInitialized = true;
    }

    currentReaction = this;
    LOG("Reaction:0x%X processing %d elements...\n", this, elements.Count());
    LOG("[");
    for (int i = 0; i < elements.Count(); i++)
    {
        LOG("%s%s", i == 0 ? " " : ", ", elements[i]->GetSymbol());
    }
    LOG(" ]\n");

    //TODO: Right now a failed compound can leave element bonds in a weird partially bonded state, which will probably FUBAR the next compound to be processed.
    Compound* newCompound = StartNewCompound();
    for (int i = 0; i < elements.Count(); i++)
    {
        LOG("Processing element %d:%s as root to compound...\n", i, elements[i]->GetSymbol());
        
        for (ReactionNode::Iterator it = CompoundDatabaseRoot.GetChildIterator(); *it; it++)
        {
            if ((*it)->Process(newCompound, elements[i], 0))
            {
                // Make a new compound for the next potential compound
                newCompound = StartNewCompound();
            }
        }
    }
    CancelCompound(newCompound);

    LOG("Reaction processing completed with %d candidate compounds.\n", possibleCompounds.Count());

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

//------------------------------------------------------------------------------
// Nodes for potential compounds
//------------------------------------------------------------------------------
//TOOD: Either make this nicer or generate it with a tool. (I'm leaning towards the latter.)
namespace PerchloricAcid
{
    RootElementSymbolNode node1("Cl");
    ElementSymbolNode node1_1("O");
    ElementSymbolNode node1_1_1("H");
    ElementSymbolNode node1_2("O");
    ElementSymbolNode node1_3("O");
    ElementSymbolNode node1_4("O");

    void Initialize()
    {
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

        CompoundDatabaseRoot.AddChild(&node1);
    }
}

void InitializeCompoundDatabase()
{
    //ElementSymbolNode node1("H");
    //ElementSymbolNode node2("H");
    //node1.AddChild(&node2);
    //node2.SetBondInfo(BondType_Covalent);
    LOG("Initializing compound database...\n");
    PerchloricAcid::Initialize();
    LOG("Done initializing compound database with %d compounds.\n", CompoundDatabaseRoot.GetChildIterator().Count());
}
