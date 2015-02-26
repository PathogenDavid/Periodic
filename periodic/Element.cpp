#include "periodic.h"
#include "Element.h"
#include "Reaction.h"
#include <sifteo.h>

// Default constructor will not create a valid element, it must be initialized before use using GetRawElement
Element::Element()
{
}

/*Constructor for element type.  We don't pass in the bond type because it will always initially be none.  */
Element::Element(const char* name, const char* symbol, groupState group, short atomicNumber, double elementWeight, int numOuterElectrons, double electroNegativity)
{
    this->baseElement = NULL;
    this->name = name;
    this->symbol = symbol;
    this->group = group;
    this->atomicNumber = atomicNumber;
    this->elementWeight = elementWeight;
    this->numOuterElectrons = numOuterElectrons;  
	this->electroNegativity = electroNegativity;
    this->sharedElectrons = 0;

    PeriodicMemset(bonds, 0, sizeof(bonds));
    this->currentReaction = NULL;
}

Element::Element(Element* baseElement)
{
    Assert(baseElement != NULL);
    this->baseElement = baseElement;
    ResetToBasicState();
}

/*Resets an element that has already changed to its original state.  */
void Element::ResetToBasicState()
{
    this->name = baseElement->name;
    this->symbol = baseElement->symbol;
    this->group = baseElement->group;
    this->atomicNumber = baseElement->atomicNumber;
    this->elementWeight = baseElement->elementWeight;
    this->numOuterElectrons = baseElement->numOuterElectrons;
    this->electroNegativity = baseElement->electroNegativity;
    this->sharedElectrons = 0;

    PeriodicMemset(bonds, 0, sizeof(bonds));
    this->currentReaction = NULL;
    this->currentCompound = NULL;
}

//Getters
const char* Element::GetName() { return name; }
const char* Element::GetSymbol() { return symbol; }
groupState Element::GetGroup() { return group; }
short Element::GetAtomicNumber() { return atomicNumber; }
double Element::GetElementWeight() { return elementWeight; }
int Element::GetNumOuterElectrons() { return numOuterElectrons; }
double Element::GetElectroNegativity() { return electroNegativity; }
int Element::GetSharedElectrons() { return sharedElectrons; }

/*Returns if the element is in its base state or not. */
bool Element::IsRawElement()
{
    return baseElement == NULL;
}

//Gets the charge of the current element
int Element::GetCharge()
{
    if (IsRawElement())
    { return 0; }

    return baseElement->numOuterElectrons - this->numOuterElectrons;
}

static Element rawElements[] =
{
    //Alkali Metals
    Element("Hydrogen", "H", HYDROGEN, 1, 1.008, 1, 2.20),
    Element("Lithium", "Li", ALKALI, 3, 6.94, 1, 0.98),
	Element("Sodium", "Na", ALKALI, 11, 22.9898, 1, 0.93),
	Element("Potassium", "K", ALKALI, 19, 39.0938, 1, 0.82),
	Element("Rubidium", "Rb", ALKALI, 37, 85.4678, 1, 0.82),
	Element("Cesium", "Cs", ALKALI, 55, 132.90545196, 1, 0.79),
    Element("Francium", "Fr", ALKALI, 87, 223.0, 1,  0.79),

    //Alkali Earth Metals
    Element("Beryllium", "Be", ALKALIEARTH, 4, 9.0121831, 2, 1.57),
    Element("Magnesium", "Mg", ALKALIEARTH, 12, 24.305, 2, 1.31),
	Element("Calcium", "Ca", ALKALIEARTH, 20, 40.078, 2, 1.0),
	Element("Strontium", "Sr", ALKALIEARTH, 38, 87.60, 2, 0.95),
	Element("Barium", "Ba", ALKALIEARTH, 56, 137.327, 2, 0.89),
    
    //Halogens
    Element("Flourine", "F", HALOGEN, 9, 18.998403163, 7, 3.98),
    Element("Chlorine", "Cl", HALOGEN, 17, 35.45, 7, 3.16),
    Element("Bromine", "Br", HALOGEN, 35, 79.094, 7, 2.96),
    Element("Iodine", "I", HALOGEN, 53, 126.90447, 7, 2.66),
	Element("Astatine", "At", HALOGEN, 85, 210, 7, 2.2), 
    
    //Noble Gases
    Element("Helium", "He", NOBLE, 2, 4.002602, 2, 0),
    Element("Neon", "Ne", NOBLE, 10, 20.1797, 8, 0),
    Element("Argon", "Ar", NOBLE, 18, 39.948, 8, 0),
    Element("Krypton", "Kr", NOBLE, 36, 83.798, 8, 0),
};

void Element::GetRawElement(int num, Element* elementOut)
{
    Assert(num >= 0 && num < GetRawElementCount());
    *elementOut = Element(&rawElements[num]);
}

bool Element::GetRawElement(const char* name, Element* elementOut)
{
    int num = GetRawElementNum(name);

    if (num < 0)
    {
        *elementOut = Element("INVALID", "INV", NONMETAL, 0, 0.0, 0, 0.0);
        return false;
    }
    else
    {
        *elementOut = Element(&rawElements[num]);
        return true;
    }
}

int Element::GetRawElementNum(const char* name)
{
    for (int i = 0; i < GetRawElementCount(); i++)
    {
        if (strcmp(name, rawElements[i].GetSymbol()) == 0)
        {
            return i;
        }
    }

    return -1;
}

int Element::GetRawElementCount()
{
    return CountOfArray(rawElements);
}

void Element::AddBond(BondSide side, Element* with)
{
    Assert(!IsRawElement());
    
    if (bonds[side].GetElement() == with)
    { return; }

    Assert(bonds[side].GetElement() == NULL);
    bonds[side] = Bond(side, with);

    // Add the bonded element to our reaction
    Assert(currentReaction != NULL);
    with->SetReaction(currentReaction);

    // Register the bond with the other element too:
    with->AddBond(Bond::GetOppositeSide(side), this);
}

void Element::SetReaction(Reaction* reaction)
{
    Assert(!IsRawElement());

    if (currentReaction == reaction)
    { return; }

    Assert(currentReaction == NULL);
    currentReaction = reaction;
    reaction->Add(this);
}

BondType Element::GetBondTypeFor(Compound* compound, BondSide side)
{
    Assert(side >= 0 && side < BondSide_Count);
    return bonds[side].GetTypeFor(compound);
}

int Element::GetBondDataFor(Compound* compound, BondSide side)
{
    Assert(side >= 0 && side < BondSide_Count);
    return bonds[side].GetDataFor(compound);
}

void Element::SetBondTypeFor(Compound* compound, BondSide side, BondType type, int data, int otherData)
{
    LOG("Element:0x%X[%s].SetTypeFor(Compound:0x%X, side=%d, type=%d, data=%d, otherData=%d)\n", this, symbol, compound, side, type, data, otherData);

    // Validate arguments
    Assert(side >= 0 && side < BondSide_Count);
    Assert(type >= 0 && type < BondType_Cound);

    // Don't apply the bond if we already did it
    if (bonds[side].GetTypeFor(compound) == type && bonds[side].GetDataFor(compound) == data)
    { return; }

    // Set the type, and set the inverse type
    compound->AddElement(this); // Add ourselves if we weren't already part of this compound.
    bonds[side].SetTypeFor(compound, type, data);

    bonds[side].GetElement()->SetBondTypeFor(compound, Bond::GetOppositeSide(side), type, otherData, data);
}

void Element::SetBondTypeFor(Compound* compound, Element* otherElement, BondType type, int data, int otherData)
{
    BondSide side = SideOf(otherElement);
    Assert(side != BondSide_Invalid);
    SetBondTypeFor(compound, side, type, data, otherData);
}

void Element::SetBondTypeFor(Compound* compound, BondSide side, BondType type, int data)
{ SetBondTypeFor(compound, side, type, data, data); }
void Element::SetBondTypeFor(Compound* compound, BondSide side, BondType type)
{ SetBondTypeFor(compound, side, type, 0, 0); }
void Element::SetBondTypeFor(Compound* compound, Element* otherElement, BondType type, int data)
{ SetBondTypeFor(compound, otherElement, type, data, data); }
void Element::SetBondTypeFor(Compound* compound, Element* otherElement, BondType type)
{ SetBondTypeFor(compound, otherElement, type, 0, 0); }

BondType Element::GetBondTypeFor(BondSide side)
{
    Assert(side >= 0 && side < BondSide_Count);
    return bonds[side].GetTypeFor(currentCompound);
}

int Element::GetBondDataFor(BondSide side)
{
    Assert(side >= 0 && side < BondSide_Count);
    return bonds[side].GetDataFor(currentCompound);
}

bool Element::HasBondType(BondType type)
{
    for (int i = 0; i < BondSide_Count; i++)
    {
        if (GetBondTypeFor((BondSide)i) == type)
        { return true; }
    }

    return false;
}

Element* Element::GetBondWith(BondSide side)
{
    Assert(side >= 0 && side < BondSide_Count);
    return bonds[side].GetElement();
}

Element* Element::GetBondWith(groupState group)
{
    for (int i = 0; i < BondSide_Count; i++)
    {
        Element* ret = GetBondWith((BondSide)i);
        if (ret != NULL && ret->GetGroup() == group)
        {
            return ret;
        }
    }

    return NULL;
}

Element* Element::GetBondWith(const char* symbol)
{
    for (int i = 0; i < BondSide_Count; i++)
    {
        Element* ret = GetBondWith((BondSide)i);
        if (ret != NULL && strcmp(ret->GetSymbol(), symbol) == 0)
        {
            return ret;
        }
    }

    return NULL;
}

ElementSet* Element::GetBondsWith(groupState group)
{
    ElementSet* ret = new ElementSet();
    for (int i = 0; i < BondSide_Count; i++)
    {
        Element* other = GetBondWith((BondSide)i);
        if (other != NULL && other->GetGroup() == group)
        {
            ret->Add(other);
        }
    }

    return ret;
}

bool Element::GetBondWith(BondSide side, Element** element_out) { *element_out = GetBondWith(side); return !!*element_out; }
bool Element::GetBondWith(groupState group, Element** element_out) { *element_out = GetBondWith(group); return !!*element_out; }
bool Element::GetBondWith(const char* symbol, Element** element_out) { *element_out = GetBondWith(symbol); return !!*element_out; }

bool Element::HasBondWith(groupState group)
{
    return GetBondWith(group) != NULL;
}

bool Element::HasBondWith(const char* symbol)
{
    return GetBondWith(symbol) != NULL;
}

BondSide Element::SideOf(Element* otherElement)
{
    for (int i = 0; i < BondSide_Count; i++)
    {
        if (GetBondWith((BondSide)i) == otherElement)
        {
            return (BondSide)i;
        }
    }

    return BondSide_Invalid;
}

void Element::ApplyCompound(Compound* compound)
{
    Assert(currentCompound == NULL);
    Assert(compound != NULL);
    
    currentCompound = compound;

    for (int i = 0; i < BondSide_Count; i++)
    {
        BondSide side = (BondSide)i;
        BondType type = GetBondTypeFor(side);
        if (type == BondType_None)
        { continue; }
        int data = GetBondDataFor(side);
        Element* other = GetBondWith(side);
        Assert(other != NULL);
        int otherData = other->GetBondDataFor(compound, Bond::GetOppositeSide(side));

        if (type == BondType_Covalent)
        {
            //TODO: Make this less terrible from a processing standpoint.
            int sharedFromMe = data - this->numOuterElectrons;
            int sharedFromOther = otherData - other->numOuterElectrons;
            
            //this->numOuterElectrons += sharedFromOther; //TODO: Should we just remove the ones we are sharing instead? - I'm just going to remove this entirely for now.
            this->sharedElectrons += sharedFromMe + sharedFromOther;

            LOG("Element:0x%X[%s].ApplyCompound(Compound:0x%X) : CovalentBond sharedFromMe=%d, sharedFromOther=%d, sharedElectrons=%d, this->numOuterElectrons=%d, other->numOuterElectrons=%d\n",
                this, symbol, compound, sharedFromMe, sharedFromOther, sharedElectrons, this->numOuterElectrons, other->numOuterElectrons
            );
        }
        else if (type == BondType_Ionic)
        {
            // Figure out who shares the electrons: (Using baseElement so the method doesn't get confused when this is applied to the other element.)
            if (this->baseElement->numOuterElectrons > other->baseElement->numOuterElectrons)
            {
                this->numOuterElectrons += 8 - this->baseElement->numOuterElectrons;
            }
            else
            {
                this->numOuterElectrons -= 8 - other->baseElement->numOuterElectrons;
            }
        }
    }
}
