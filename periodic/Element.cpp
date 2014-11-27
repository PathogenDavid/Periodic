#include "periodic.h"
#include "Element.h"
#include <sifteo.h>

//Enumeration to keep track of the state of element at the current bonding process.  Potential is in there to show if a bond may have the potential to bond.
enum bondState { NONE, IONIC, COVALENT,  POTENTIAL};

// Default constructor will not create a valid element, it must be initialized before use using GetRawElement
Element::Element()
{
}


//Constructor for element type.  We don't pass in the bond type because it will always initially be none.  
Element::Element(const char* name, const char* symbol, const char* group, short atomicNumber, double elementWeight, int numOuterElectrons, double electroNegativity)
{
    this->baseElement = NULL;
    this->name = name;
    this->symbol = symbol;
	this->group = group;
    this->atomicNumber = atomicNumber;
    this->elementWeight = elementWeight;
    this->numOuterElectrons = numOuterElectrons;
	this->electroNegativity = electroNegativity;
	this->bondType = NONE;
}

Element::Element(Element* baseElement)
{
    Assert(baseElement != NULL);
    this->baseElement = baseElement;
    ResetToBasicState();
}

void Element::ResetToBasicState()
{
    this->name = baseElement->name;
    this->symbol = baseElement->symbol;
    this->atomicNumber = baseElement->atomicNumber;
    this->elementWeight = baseElement->elementWeight;
    this->numOuterElectrons = baseElement->numOuterElectrons;
	this->bondType = NONE;
}

const char* Element::GetName() { return name; }
const char* Element::GetSymbol() { return symbol; }
const char* Element::GetGroup() { return group; }
short Element::GetAtomicNumber() { return atomicNumber; }
double Element::GetElementWeight() { return elementWeight; }
int Element::GetNumOuterElectrons() { return numOuterElectrons; }
double Element::GetElectroNegativity() { return electroNegativity; }
int Element::GetBondType() { return bondType; } 

bool Element::IsRawElement()
{
    return baseElement == NULL;
}

int Element::GetCharge()
{
    if (IsRawElement())
    { return 0; }

    return baseElement->numOuterElectrons - this->numOuterElectrons;
}

bool Element::ReactWith(Element* other)
{
    //TODO: This method needs to become more complicated to where it stores state about what elements it is interacting with.
    //LOG("My electrons: %d, Other electrons: %d\n", this->numOuterElectrons, other->numOuterElectrons);

	//if any element is a noble gas, a bond won't occur.
	if (strcmp(this->group, "noble") == 0 ||
		strcmp(other->group, "noble") == 0)
		return false;
	//if both elements are alkali metals, no bonding will occur
	else if (strcmp(this->group, "alkali") == 0 &&
		strcmp(other->group, "alkali") == 0)
		return false;
	//if both elements are halogens, covalent bonding will occur
	else if (strcmp(this->group, "halogen") == 0 &&
		strcmp(other->group, "halogen") == 0)
		return true;
	//hydrogen will bond with any halogen and form a covalent bond
	else if (strcmp(this->name, "Hydrogen") == 0 &&
		strcmp(other->group, "halogen") == 0)
		return true;
	//The difference in electronegativity is 1.68, which is very near to 1.7
	//This is an Ionic bond but a special case in which the difference isn't >= 1.7
	//which is why we have a hard coded case (possible to clean up in the future?)
	else if (strcmp(this->name, "Lithium") == 0 &&
		strcmp(other->name, "Iodine") == 0)
		return true;

	//find the greater negativity
	double maxNegativity = this->electroNegativity > other->electroNegativity ? this->electroNegativity : other->electroNegativity;
	double minNegativity = this->electroNegativity < other->electroNegativity ? this->electroNegativity : other->electroNegativity;

	//Ionic
	if (maxNegativity - minNegativity > 1.7 &&
		this->numOuterElectrons + other->numOuterElectrons == 8)
	{
		if (this->numOuterElectrons < other->numOuterElectrons)
		{
			int electronsDonated = 8 - other->numOuterElectrons;
			this->numOuterElectrons -= electronsDonated;
			other->numOuterElectrons += electronsDonated;
		}
		else
		{
			int electronsDonated = 8 - this->numOuterElectrons;
			other->numOuterElectrons -= electronsDonated;
			this->numOuterElectrons += electronsDonated;
		}
		return true;
	}
		
	//covalent
	else if (maxNegativity - minNegativity <= 1.7)
		return true;

	return false;
}

static Element rawElements[] =
{
    //Alkali Metals
	Element("Hydrogen", "H", "nonmetal", 1, 1.008, 1, 2.20),
    Element("Lithium", "Li", "alkali", 3, 6.94, 1, 0.98),
	Element("Sodium", "Na", "alkali", 11, 22.9898, 1, 0.93),
	Element("Potassium", "K", "alkali", 19, 39.0938, 1, 0.82),
	Element("Rubidium", "Rb", "alkali", 37, 85.4678, 1, 0.82),
	Element("Cesium", "Cs", "alkali", 55, 132.90545196, 1, 0.79), //don't remember if we need this one or not
    
    //Halogens
    Element("Flourine", "F", "halogen", 9, 18.998403163, 7, 3.98),
	Element("Chlorine", "Cl", "halogen", 17, 35.45, 7, 3.16),
	Element("Bromine", "Br", "halogen", 35, 79.094, 7, 2.96),
	Element("Iodine", "I", "halogen", 53, 126.90447, 7, 2.66),
    
    //Noble Gases
    Element("Helium", "He", "noble", 2, 4.002602, 2, 0),
	Element("Neon", "Ne", "noble", 10, 20.1797, 8, 0),
	Element("Argon", "Ar", "noble", 18, 39.948, 8, 0),
	Element("Krypton", "Kr", "noble", 36, 83.798, 8, 0),
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
        *elementOut = Element("INVALID", "INV", "INV", 0, 0.0, 0, 0.0);
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
