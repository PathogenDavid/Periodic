#include "Reaction.h"
#include "Element.h"
#include "Trie.h"

Reaction::Reaction()
{
}

Reaction::~Reaction()
{
    for (int i = 0; i < possibleCompounds.Count(); i++)
    { delete possibleCompounds[i]; }
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
    Compound* ret = new Compound(possibleCompounds.Count());
    possibleCompounds.Add(ret);
    return ret;
}

void Reaction::CancelCompound(Compound* compound)
{
    possibleCompounds.Remove(compound);
    delete compound;
}

// Trie Initialization
bool Reaction::trieIsInitialized;
Trie Reaction::trie;
void Reaction::InitializeTrie()
{
    if (trieIsInitialized)
    { return; }
    
    trie.addBond("HH", COVALENT);
    trie.addBond("LiH", COVALENT);
    trie.addBond("NaH", COVALENT);
    trie.addBond("KH", COVALENT);
    trie.addBond("CsH", COVALENT);
    trie.addBond("FrH", COVALENT);
    trie.addBond("FF", COVALENT);
    trie.addBond("ClCl", COVALENT);
    trie.addBond("BrBr", COVALENT);
    trie.addBond("II", COVALENT);
    trie.addBond("FCl", COVALENT);
    trie.addBond("FBr", COVALENT);
    trie.addBond("FI", COVALENT);
    trie.addBond("ClBr", COVALENT);
    trie.addBond("ClI", COVALENT);
    trie.addBond("HF", COVALENT);
    trie.addBond("HCl", COVALENT);
    trie.addBond("HBr", COVALENT);
    trie.addBond("HI", COVALENT);
    trie.addBond("HAt", COVALENT);
    trie.addBond("LiF", IONIC);
    trie.addBond("LiCl", IONIC);
    trie.addBond("LiBr", IONIC);
    trie.addBond("LiI", IONIC);
    trie.addBond("NaF", IONIC);
    trie.addBond("NaCl", IONIC);
    trie.addBond("NaBr", IONIC);
    trie.addBond("NaI", IONIC);
    trie.addBond("KF", IONIC);
    trie.addBond("KCl", IONIC);
    trie.addBond("KBr", IONIC);
    trie.addBond("KI", IONIC);
    trie.addBond("RbF", IONIC);
    trie.addBond("RbCl", IONIC);
    trie.addBond("RbBr", IONIC);
    trie.addBond("RbI", IONIC);
    trie.addBond("CsF", IONIC);
    trie.addBond("CsCl", IONIC);
    trie.addBond("CsBr", IONIC);
    trie.addBond("CsI", IONIC);
    trie.addBond("FrF", IONIC);
    trie.addBond("FrCl", IONIC);
    trie.addBond("FrBr", IONIC);
    trie.addBond("FrI", IONIC);
    trie.addBond("BeHH", COVALENT);
    trie.addBond("MgHH", COVALENT);
    trie.addBond("CaHH", IONIC);
    trie.addBond("SrHH", IONIC);
    trie.addBond("BaHH", IONIC);
    trie.addBond("BeFF", COVALENT);
    trie.addBond("BeClCl", COVALENT);
    trie.addBond("BeBrBr", COVALENT);
    trie.addBond("BeII", COVALENT);
    trie.addBond("MgFF", COVALENT);
    trie.addBond("MgClCl", COVALENT);
    trie.addBond("MgBrBr", COVALENT);
    trie.addBond("MgII", COVALENT);
    trie.addBond("CaFF", COVALENT);
    trie.addBond("CaClCl", COVALENT);
    trie.addBond("CaBrBr", COVALENT);
    trie.addBond("CaII", COVALENT);
    trie.addBond("SrFF", COVALENT);
    trie.addBond("SrClCl", COVALENT);
    trie.addBond("SrBrBr", COVALENT);
    trie.addBond("SrII", COVALENT);
    trie.addBond("BaFF", COVALENT);
    trie.addBond("BaClCl", COVALENT);
    trie.addBond("BaBrBr", COVALENT);
    trie.addBond("BaII", COVALENT);
    trie.addBond("SiOO", COVALENT);
    trie.addBond("SO", COVALENT);
    trie.addBond("SOO", COVALENT);
    trie.addBond("SOOO", COVALENT);
    trie.addBond("SSO", COVALENT);
    trie.addBond("SSOO", COVALENT);
    trie.addBond("HClO", COVALENT);
    trie.addBond("HBrO", COVALENT);

    trieIsInitialized = true;
}
