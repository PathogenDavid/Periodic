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

#if 0
/*Checks to see if two elements will react.  If they do react, we need to change the bondType of the elements to show which way they react.  
In a Ionic type reaction, we need to show which element gives an electron to the other element and update their respective counts.
In certain cases where a bond would occur if another element (for now the same kind as one of the two in question) then we change 
the bondType to POTENTIAL.

This algorithm was written with help by:
Christopher Culbertson, Associate Professor at Kansas State University
Michael Ayala, Chemistry Major at UC Davis */
bool Element::ReactWith(Element* other)
{
    //TODO: This method needs to become more complicated to where it stores state about what elements it is interacting with.
    //LOG("My electrons: %d, Other electrons: %d\n", this->numOuterElectrons, other->numOuterElectrons);

	// if any element is a noble gas, return immedeatly
	if ( this->group == NOBLE || other->group == NOBLE)
		return false;
	
	// figure out which group the first element is in and make a switch out of it
	// as to avoid trudging through if after if after if statement
	// and because we like efficiency :)
	int firstElement = this->group;
	switch(firstElement)
	{
		// If the first element is Hydrogen
		case HYDROGEN:
		{
			// if we have 2 hydrogens, they form a covalent bond
			if(other->group == HYDROGEN)
			{
				this->bondType = COVALENT;
				other->bondType = COVALENT;
				break;
			}
			// hydrogen will bond with any halogen and form a covalent bond
			else if (other->group == HALOGEN)
			{
				this->bondType = COVALENT;
				other->bondType = COVALENT;
				break;
			}

			else if (other->group == ALKALI)
			{
				this->bondType = COVALENT;
				other->bondType = COVALENT;
				break;
			}
			// Hydrogen and any alkali earth have the potential to bond (such as another hydrogen)
			// but cannot bond on their own.
			else if (other->group == ALKALIEARTH)
			{
				this->bondType = POTENTIAL;
				other->bondType = POTENTIAL;
				return false;
			}

			break;
		}
		// If the first element is a halogen
		case HALOGEN:
		{
			// if both elements are halogens, covalent bonding will occur
			if(other->group == HALOGEN)
			{
				this->bondType = COVALENT;
				other->bondType = COVALENT;
				break;
			}
			// hydrogen will bond with any halogen and form a covalent bond
			else if (other->group == HYDROGEN)
			{
				this->bondType = COVALENT;
				other->bondType = COVALENT;
				break;
			}
			else if (other->group == ALKALI)
			{
				this->bondType = IONIC;
				other->bondType = IONIC;
				break;
			}
			// if we have an alkali earth metal and a halogen we have the potential for a bond
			else if (other->group == ALKALIEARTH)
			{
				this->bondType = POTENTIAL;
				other->bondType = POTENTIAL;
				return false;
			}
			/*
			// unfortuantly we need to hardcode in this case as it doens't follow the standard rule set
			else if (strcmp(this->symbol, "I") == 0 &&
				strcmp(other->symbol, "Li") == 0)
			{
				this->bondType = IONIC;
				other->bondType = IONIC;
			}
			*/
			break;

		}
		//If the first element is an alkali metal
		case ALKALI:
		{
			// if two alkali metals, nothing will occur
			if(other->group == ALKALI)
				return false;

			else if (other->group == HYDROGEN)
			{
				this->bondType = COVALENT;
				other->bondType = COVALENT;
				break;
			}

			else if (other->group == HALOGEN)
			{
				this->bondType = IONIC;
				other->bondType = IONIC;
				break;
			}

			/*
			// unfortuantly we need to hardcode in this case as it doens't follow the standard rule set
			else if (strcmp(this->symbol, "Li") == 0 &&
				strcmp(other->symbol, "I") == 0)
			{
				this->bondType = IONIC;
				other->bondType = IONIC;
			}
			*/
			break;

		}
		// if the first element is an alkali earth metal
		case ALKALIEARTH:
		{
			// if we have an alkali earth metal and a halogen, we have the potential for a bond
			if(other->group == HALOGEN)
			{
				this->bondType = POTENTIAL;
				other->bondType = POTENTIAL;
				return false;
			}
			// if we have an alkali earth metal and a halogen, we have the potential for a bond
			else if (other->group == HYDROGEN)
			{
				this->bondType = POTENTIAL;
				other->bondType = POTENTIAL;
				return false;
			}
			break;
		}
		default:
			break;
	}

	//logic to add/share electrons
	int typeOfBond = this->bondType;
	switch(typeOfBond)
	{
		case COVALENT:
		{
			// Covalent bonds are bonds in which electrons are shared between each other.  
			// Therefore, we take the maxmimum number of electrons allowed in the outer shell 
			// (2 for hydrogen and helium, 8 for everything else) and subtract from that the number of
			// current outer electrons.  Then we will subject that value from the actual number of outer
			// electrons (effectively making it zero).  Then we do the same process for the second element
			// and add the two shared values together.  For our purposes here and now, that value will be 2.
			// we will then add that shared combined value to both elements outer electrons shell and we'll
			// get a full outer shell.
			int sharedElectronsElem1;
			int sharedElectronsElem2;
			//first element
			if(this->group == HYDROGEN)
			{
				sharedElectronsElem1 = 2 - this->numOuterElectrons;
				this->numOuterElectrons -= sharedElectronsElem1;
			}
			else if (this->group == ALKALI)
			{
				sharedElectronsElem1 = 1;
				this->numOuterElectrons -= sharedElectronsElem1;
			}
			else
			{
				sharedElectronsElem1 = 8 - this->numOuterElectrons;
				this->numOuterElectrons -= sharedElectronsElem1;
			}
			//second element
			if(other->group == HYDROGEN)
			{
				sharedElectronsElem2 = 2 - other->numOuterElectrons;
				other->numOuterElectrons -= sharedElectronsElem2;
			}
			else if (other->group == ALKALI)
			{
				sharedElectronsElem2 = 1;
				other->numOuterElectrons -= sharedElectronsElem2;
			}
			else
			{
				sharedElectronsElem2 = 8 - other->numOuterElectrons;
				other->numOuterElectrons -= sharedElectronsElem2;
			}
			
			int shared = sharedElectronsElem1 + sharedElectronsElem2;
			this->numOuterElectrons += shared;
			other->numOuterElectrons += shared;
			this->sharedElectrons = shared;
			other->sharedElectrons = shared;
			return true;
		}
		case IONIC:
		{
			// Ionic bonds are bonds in which electrons are donated from one element to the next.
			// 
			int electronsDonated;
			//determine which element shares electrons
			if(this->numOuterElectrons > other->numOuterElectrons)
			{
				electronsDonated = 8 - this->numOuterElectrons;
				this->numOuterElectrons += electronsDonated;
				other->numOuterElectrons -= electronsDonated;
			}
			else 
			{
				electronsDonated = 8 - other->numOuterElectrons;
				other->numOuterElectrons += electronsDonated;
				this->numOuterElectrons -= electronsDonated;
			}
			return true;
		}
		default:
		{
			return false;
		}
	}
}



/* Checks reactions that require 3 elements to form a bond) 

This algorithm was written with help by:
Christopher Culbertson, Associate Professor at Kansas State University
Michael Ayala, Chemistry Major at UC Davis */
bool Element::ReactWith(Element* other1, Element* other2)
{
    bool containsAlkaliEarth = false;
	int alkaliEarthPosition;
	
	// Element array used so we can keep track of which element is at what position.  
	// Consequently, we don't have to do a lot of pointless statements to keep checking
	// that position
	Element *elementArray[3] = {this, other1, other2};

	// initial check to see if we have an alkali earth (currently we only have 3 element bonding if
	// there is an alkali earth metal.  If there is, we mark which element it is to make our lives
	// easier later on
	if(this->group == ALKALIEARTH)
	{
		containsAlkaliEarth = true;
		alkaliEarthPosition = 0;
	}
	else if (other1->group == ALKALIEARTH)
	{
		containsAlkaliEarth = true;
		alkaliEarthPosition = 1;
	}
	else if (other2->group == ALKALIEARTH)
	{
		containsAlkaliEarth = true;
		alkaliEarthPosition = 2;
	}

	if(containsAlkaliEarth)
	{
		//get the positions of the other elements in the array
		int otherElement1Position = (alkaliEarthPosition + 1) % 3;
		int otherElement2Position = (alkaliEarthPosition + 2) % 3;

		//check to see if the other two elements are halogens (leads to covalent bonding)
		bool containsTwoHalogens = elementArray[otherElement1Position]->group == HALOGEN && 
			elementArray[otherElement2Position]->group == HALOGEN;

		if(containsTwoHalogens)
		{

				//Logic to see how many electrons are shared with each element
				elementArray[alkaliEarthPosition]->sharedElectrons = 4;
				elementArray[otherElement1Position]->sharedElectrons = 2;
				elementArray[otherElement2Position]->sharedElectrons = 2;

				//set the bond type
				elementArray[alkaliEarthPosition]->bondType = COVALENT;
				elementArray[otherElement1Position]->bondType = COVALENT;
				elementArray[otherElement2Position]->bondType = COVALENT;
				
				//we have a covalent bond, return
				return true;
		}

		//check to see if the other two elements are hydrogen (leads to either covalent or ionic bonding)
		bool containsTwoHydrogens = elementArray[otherElement1Position]->group == HYDROGEN && 
			elementArray[otherElement2Position]->group == HYDROGEN;
			
		if(containsTwoHydrogens)
		{
				//If the alkali earth metals are either Beryllium or Magnesium, the bond will be covalent
				//If they are the other alkali earth metals, they will make an ionic bond
				
				if(strcmp(elementArray[alkaliEarthPosition]->symbol, "Be") == 0 ||
					(strcmp(elementArray[alkaliEarthPosition]->symbol, "Mg") == 0))
				{
					//logic to see how many electrons are shared with each element
					elementArray[alkaliEarthPosition]->sharedElectrons = 4;
					elementArray[otherElement1Position]->sharedElectrons = 2;
					elementArray[otherElement2Position]->sharedElectrons = 2;

					//set the bond type
					elementArray[alkaliEarthPosition]->bondType = COVALENT;
					elementArray[otherElement1Position]->bondType = COVALENT;
					elementArray[otherElement2Position]->bondType = COVALENT;

					//we have a covalent bond, return
					return true;
				}
				else
				{
					//logic to see how any electrons are donated
					int electronsDonated = 1;
					elementArray[alkaliEarthPosition]->numOuterElectrons -= 2*electronsDonated;
					elementArray[otherElement1Position]->numOuterElectrons += electronsDonated;
					elementArray[otherElement2Position]->numOuterElectrons += electronsDonated;
					
					//set the bond type
					elementArray[alkaliEarthPosition]->bondType = IONIC;
					elementArray[otherElement1Position]->bondType = IONIC;
					elementArray[otherElement2Position]->bondType = IONIC;
					
					//we have an ionic bond, return 
					return true;
				}
		}
	}

	return false;
}
#endif

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
        int otherData = other->GetBondDataFor(Bond::GetOppositeSide(side));

        if (type == BondType_Covalent)
        {
            int sharedFromMe = data - this->numOuterElectrons;
            int sharedFromOther = otherData - other->numOuterElectrons;
            
            //this->numOuterElectrons += sharedFromOther; //TODO: Should we just remove the ones we are sharing instead? - I'm just going to remove this entirely for now.
            this->sharedElectrons += sharedFromMe + sharedFromOther;
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
