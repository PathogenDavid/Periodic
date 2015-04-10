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

    virtual bool ProcessChildren(Compound* compound, Element* inputForChildren, int depth)
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
        //LOG("%s:0x%X.Process(Compound:0x%X, Element:0x%X[%s], %d)\n", GetDescription(), this, compound, input, input->GetSymbol(), depth);
        Assert(depth >= 0 && depth < MAX_REACTION_DEPTH);
        bool success = false;
        Element* output = NULL;
        Element* lastOutput = NULL;

        while (true)
        {
            // This will return a different element each time it is called, and NULL when no element is available
            // Therefore, we can loop until we find an element with child elements that satisfy this branch of the reaction.
            lastOutput = output;
            output = GetOutput(compound, input, depth);

            // If output == NULL, we ran out of possibilities and failed.
            // If output == lastOutput, then either the node has a flawed implementation, or (more likely) it is a dummy pass-through node. Either way we want to not loop forever.
            if (output == NULL || output == lastOutput)
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

    virtual const char* GetDescription() { return "ReactionNode"; }
};

class ElementSymbolFilterNode : public ReactionNode
{
private:
    const char* symbol;
public:
    ElementSymbolFilterNode(const char* symbol)
    {
        this->symbol = symbol;
        LOG("ElementSymbolFilterNode:0x%X %s\n", this, symbol);
    }

    virtual Element* GetOutput(Compound* compound, Element* input, int depth)
    {
        return strcmp(input->GetSymbol(), symbol) == 0 ? input : NULL;
    }

    virtual const char* GetDescription() { return "ElementSymbolFilterNode"; }
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
        return input->GetBondWith(symbol);
    }

    virtual const char* GetDescription() { return "ElementSymbolNode"; }
};

class ElementGroupFilterNode : public ReactionNode
{
private:
    groupState group;
public:
    ElementGroupFilterNode(groupState group)
    {
        this->group = group;
        LOG("ElementGroupFilterNode:0x%X %d\n", this, group);
    }

    virtual Element* GetOutput(Compound* compound, Element* input, int depth)
    {
        return input->GetGroup() == group ? input : NULL;
    }

    virtual const char* GetDescription() { return "ElementGroupFilterNode"; }
};

class ElementGroupNode : public ReactionNode
{
private:
    groupState group;
public:
    ElementGroupNode(groupState group)
    {
        this->group = group;
        LOG("ElementGroupNode:0x%X %d\n", this, group);
    }

    virtual Element* GetOutput(Compound* compound, Element* input, int depth)
    {
        return input->GetBondWith(group);
    }

    virtual const char* GetDescription() { return "ElementGroupNode"; }
};

// A logical node that only needs one branch of children to succeed
//NOTE: This should not be used to implement alternative, inclusive reactions as it will not even try the other children once one succeeds.
class EitherOrNode : public ReactionNode
{
public:
    virtual bool ProcessChildren(Compound* compound, Element* inputForChildren, int depth)
    {
        for (Iterator it(&firstChild); *it; it++)
        {
            if ((*it)->Process(compound, inputForChildren, depth))
            {
                return true; // Return true when the first child succeeds.
            }
        }

        return false; // If we got this far, then no children succeedeed.
    }

    virtual Element* GetOutput(Compound* compound, Element* input, int depth)
    {
        // This node is a pure pass-through node
        return input;
    }

    virtual const char* GetDescription() { return "EitherOrNode"; }
};

//! This node doesn't do anything other than pass the element through, used for coalescing common elements after an EitherOrNode.
class NoOpNode : public ReactionNode
{
public:
    virtual Element* GetOutput(Compound* compound, Element* input, int depth)
    {
        // This node is a pure pass-through node
        return input;
    }

    virtual const char* GetDescription() { return "NoOpNode"; }
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

    // Fail immediately if there's only one element in this reaction
    if (elements.Count() == 1)
    { return false; }

    // Debug printing
    LOG("Reaction:0x%X processing %d elements...\n", this, elements.Count());
    LOG("[ ");
    for (int i = 0; i < elements.Count(); i++)
    {
        if (i > 0)
        { LOG(", "); }
        LOG("%s", elements[i]->GetSymbol());
    }
    LOG(" ]\n");

    //TODO: Right now a failed compound can leave element bonds in a weird partially bonded state, which will probably FUBAR the next compound to be processed.
    currentReaction = this;
    for (int i = 0; i < elements.Count(); i++)
    {
        LOG("Processing element %d:%s as root to compound...\n", i, elements[i]->GetSymbol());
        
        for (ReactionNode::Iterator it = CompoundDatabaseRoot.GetChildIterator(); *it; it++)
        {
            Compound* newCompound = StartNewCompound();
            if (!(*it)->Process(newCompound, elements[i], 0))
            { CancelCompound(newCompound); } // Cancel the compound if the process was not successful.
        }
    }

    LOG("Reaction processing completed with %d candidate compounds.\n", possibleCompounds.Count());

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
    ElementSymbolFilterNode root("Cl");
    ElementSymbolNode node_1("O");
    ElementSymbolNode node_1_1("H");
    ElementSymbolNode node_2("O");
    ElementSymbolNode node_3("O");
    ElementSymbolNode node_4("O");

    void Initialize()
    {
        root.AddChild(&node_1);
        root.AddChild(&node_2);
        root.AddChild(&node_3);
        root.AddChild(&node_4);

        node_1.AddChild(&node_1_1);

        node_1.SetBondInfo(BondType_Covalent, 1);
        node_2.SetBondInfo(BondType_Covalent, 2);
        node_3.SetBondInfo(BondType_Covalent, 2);
        node_4.SetBondInfo(BondType_Covalent, 2);

        node_1_1.SetBondInfo(BondType_Covalent, 1);

        CompoundDatabaseRoot.AddChild(&root);
    }
}

namespace Acetylene
{
    ElementSymbolFilterNode h1("H");
    ElementSymbolNode c1("C");
    ElementSymbolNode c2("C");
    ElementSymbolNode h2("H");

    void Initialize()
    {
        h1.AddChild(&c1);
        c1.AddChild(&c2);
        c2.AddChild(&h2);

        c1.SetBondInfo(BondType_Covalent, 1);
        c2.SetBondInfo(BondType_Covalent, 3);
        h2.SetBondInfo(BondType_Covalent, 1);

        CompoundDatabaseRoot.AddChild(&h1);
    }
}

namespace DisulfurDioxide
{
    ElementSymbolFilterNode o1("O");
    ElementSymbolNode s1("S");
    ElementSymbolNode s2("S");
    ElementSymbolNode o2("O");

    void Initialize()
    {
        o1.AddChild(&s1);
        s1.AddChild(&s2);
        s2.AddChild(&o2);

        s1.SetBondInfo(BondType_Covalent, 2);
        s2.SetBondInfo(BondType_Covalent, 1);
        o2.SetBondInfo(BondType_Covalent, 2);

        CompoundDatabaseRoot.AddChild(&o1);
    }
}

namespace PhosphorousAcid1 /* H3PO3 Tautomer */
{
    ElementSymbolFilterNode p("P");
    ElementSymbolNode o1("O");
    ElementSymbolNode o2("O");
    ElementSymbolNode o3("O");
    ElementSymbolNode h1("H");
    ElementSymbolNode h2("H");
    ElementSymbolNode h3("H");

    void Initialize()
    {
        p.AddChild(&o1);
        p.AddChild(&o2);
        p.AddChild(&o3);

        o1.AddChild(&h1);
        o2.AddChild(&h2);
        o3.AddChild(&h3);

        o1.SetBondInfo(BondType_Covalent, 1);
        o2.SetBondInfo(BondType_Covalent, 1);
        o3.SetBondInfo(BondType_Covalent, 1);
        h1.SetBondInfo(BondType_Ionic);
        h2.SetBondInfo(BondType_Ionic);
        h3.SetBondInfo(BondType_Ionic);

        CompoundDatabaseRoot.AddChild(&p);
    }
}

namespace PhosphorousAcid2 /* H2PHO3 Tautomer */
{
    ElementSymbolFilterNode p("P");
    ElementSymbolNode o1("O");
    ElementSymbolNode o2("O");
    ElementSymbolNode o3("O");
    ElementSymbolNode h1("H");
    ElementSymbolNode h2("H");
    ElementSymbolNode h3("H");

    void Initialize()
    {
        p.AddChild(&o1);
        p.AddChild(&o2);
        p.AddChild(&o3);
        p.AddChild(&h1);

        o2.AddChild(&h2);
        o3.AddChild(&h3);

        o1.SetBondInfo(BondType_Covalent, 2);
        o2.SetBondInfo(BondType_Covalent, 1);
        o3.SetBondInfo(BondType_Covalent, 1);
        h1.SetBondInfo(BondType_Covalent, 1);
        h2.SetBondInfo(BondType_Ionic);
        h3.SetBondInfo(BondType_Ionic);

        CompoundDatabaseRoot.AddChild(&p);
    }
}

//------------------------------------------------------------------------------
// Two Element Reactions
//------------------------------------------------------------------------------
namespace Hydrogen_HydrogenOrHalogenOrAlkali
{
    //Hydrogen - Hydrogen
    ElementGroupFilterNode h_h1(HYDROGEN);
    ElementGroupNode h_h2(HYDROGEN);

    // Hydrogen - Halogen
    ElementGroupFilterNode h_hal1(HYDROGEN);
    ElementGroupNode h_hal2(HALOGEN);

    // Hydrogen - Alkali
    ElementGroupFilterNode h_a1(HYDROGEN);
    ElementGroupNode h_a2(ALKALI);

    void Initialize()
    {
        h_h1.AddChild(&h_h2);
        h_h2.SetBondInfo(BondType_Covalent, 1);
        CompoundDatabaseRoot.AddChild(&h_h1);

        h_hal1.AddChild(&h_hal2);
        h_hal2.SetBondInfo(BondType_Covalent, 1);
        CompoundDatabaseRoot.AddChild(&h_hal1);

        h_a1.AddChild(&h_a2);
        h_a2.SetBondInfo(BondType_Ionic);
        CompoundDatabaseRoot.AddChild(&h_a1);
    }
}

namespace Halogen_HalogenOrAlkali
{
    // Halogen - Halogen
    ElementGroupFilterNode ha_ha1(HALOGEN);
    ElementGroupNode ha_ha2(HALOGEN);

    // Halogen - Alkali
    ElementGroupFilterNode ha_a1(HALOGEN);
    ElementGroupNode ha_a2(ALKALI);

    void Initialize()
    {
        ha_ha1.AddChild(&ha_ha2);
        ha_ha2.SetBondInfo(BondType_Covalent, 1);
        CompoundDatabaseRoot.AddChild(&ha_ha1);

        ha_a1.AddChild(&ha_a2);
        ha_a2.SetBondInfo(BondType_Ionic);
        CompoundDatabaseRoot.AddChild(&ha_a1);
    }
}

namespace LithiumIodine
{
    ElementSymbolFilterNode root("Li");
    ElementSymbolNode node_1("I");

    void Initialize()
    {
        root.AddChild(&node_1);
        node_1.SetBondInfo(BondType_Ionic);
        CompoundDatabaseRoot.AddChild(&root);
    }
}

//------------------------------------------------------------------------------
// Three Element Reactions
//------------------------------------------------------------------------------
namespace AlkaliEarth_2HalogenOr2Hydrogen
{
    ElementGroupFilterNode halogenRoot(ALKALIEARTH);
    EitherOrNode or1; // Used as if-else for Be
        ElementSymbolFilterNode beFilter("Be");
        ElementGroupNode be_1(HALOGEN);
        ElementGroupNode be_2(HALOGEN);
        /* else */
        NoOpNode elseNode;
            ElementGroupNode else_1(HALOGEN);
            ElementGroupNode else_2(HALOGEN);

    void Initialize()
    {
        halogenRoot.AddChild(&or1);
        or1.AddChild(&beFilter);
        {
            //If the alkali earth metal is Beryllium, the bond will be covalent
            beFilter.AddChild(&be_1);
            beFilter.AddChild(&be_2);
            be_1.SetBondInfo(BondType_Covalent, 1);
            be_2.SetBondInfo(BondType_Covalent, 1);
        }
        or1.AddChild(&elseNode);
        {
            //If they are the other alkali earth metals, they will make an ionic bond
            elseNode.AddChild(&else_1);
            elseNode.AddChild(&else_2);
            else_1.SetBondInfo(BondType_Ionic);
            else_2.SetBondInfo(BondType_Ionic);
        }

        CompoundDatabaseRoot.AddChild(&halogenRoot);
    }
}

void InitializeCompoundDatabase()
{
    LOG("Initializing compound database...\n");
    PerchloricAcid::Initialize();
    Hydrogen_HydrogenOrHalogenOrAlkali::Initialize();
    Halogen_HalogenOrAlkali::Initialize();
    LithiumIodine::Initialize();
    AlkaliEarth_2HalogenOr2Hydrogen::Initialize();
    Acetylene::Initialize();
    DisulfurDioxide::Initialize();
    PhosphorousAcid1::Initialize();
    PhosphorousAcid2::Initialize();
    LOG("Done initializing compound database with %d compounds.\n", CompoundDatabaseRoot.GetChildIterator().Count());
}
