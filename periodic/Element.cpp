#include "periodic.h"
#include "Element.h"
#include <sifteo.h>

/*Enumeration to keep track of the state of element at the current bonding process.  Potential is in there to show if a bond may have the potential to bond. */
enum bondState { NONE, IONIC, COVALENT,  POTENTIAL}; 

// Default constructor will not create a valid element, it must be initialized before use using GetRawElement
Element::Element()
{
}


/*Constructor for element type.  We don't pass in the bond type because it will always initially be none.  */
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
    this->shared = 0;
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
	this->bondType = NONE;
    this->shared = 0;
    this->electroNegativity = baseElement->electroNegativity;
}

//Getters
const char* Element::GetName() { return name; }
const char* Element::GetSymbol() { return symbol; }
const char* Element::GetGroup() { return group; }
short Element::GetAtomicNumber() { return atomicNumber; }
double Element::GetElementWeight() { return elementWeight; }
int Element::GetNumOuterElectrons() { return numOuterElectrons; }
double Element::GetElectroNegativity() { return electroNegativity; }
int Element::GetBondType() { return bondType; } 
int Element::GetShared() { return shared; }

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

	//if any element is a noble gas, a bond won't occur.
	if (strcmp(this->group, "noble") == 0 ||
		strcmp(other->group, "noble") == 0)
		return false;
		
	//if both elements are alkali metals, no bonding will occur
	else if (strcmp(this->group, "alkali") == 0 &&
		strcmp(other->group, "alkali") == 0)
		return false;

    //if we have 2 hydrogens, they have the potential to become hydrides with an alkali earth metal
    else if (strcmp(this->symbol, "H") == 0 &&
        strcmp(other->symbol, "H") == 0)
    {
        this->bondType = POTENTIAL;
        other->bondType = POTENTIAL;
        return false;
    }

		
	//if both elements are halogens, covalent bonding will occur
	else if (strcmp(this->group, "halogen") == 0 &&
		strcmp(other->group, "halogen") == 0)
	{   
        //If we have two of the same halogen, they can pair with an alkali earth metal to form a compoud.
        //But, for example, F2 is Flourine Gas, which I guess is technically a compound.  If we want
        //To Say its a compoud, leave the uncommented part uncommented.  I'm keeping it in for now.
        /*
        if(strcmp(this->name, other->name) == 0)
        {
            this->bondType = POTENTIAL;
            other->bondType = POTENTIAL;
            return false;
        }
        */

        //for any halogen bonding with another halogen, 2 electrons will be shared between the two.
        int sharedElectronsElem1 = 8 - this->numOuterElectrons;
        this->numOuterElectrons -= sharedElectronsElem1;
        int sharedElectronsElem2 = 8 - other->numOuterElectrons;
        other->numOuterElectrons -+ sharedElectronsElem2;

        int shared = sharedElectronsElem1 + sharedElectronsElem2;
        this->numOuterElectrons += shared;
        other->numOuterElectrons += shared;

        this->shared = shared;
        other->shared = shared;
        this->bondType = COVALENT;
        other->bondType = COVALENT;
		return true;
	}
		
	//hydrogen will bond with any halogen and form a covalent bond
	else if (strcmp(this->symbol, "H") == 0 &&
		strcmp(other->group, "halogen") == 0)
    {
        //for any halogen bonding with hydrogen, 2 electrons will be shared between the two.
        int sharedElectronsElem1 = 8 - this->numOuterElectrons;
        this->numOuterElectrons -= sharedElectronsElem1;
        int sharedElectronsElem2 = 2 - other->numOuterElectrons;
        other->numOuterElectrons -= sharedElectronsElem2;

        int shared = sharedElectronsElem1 + sharedElectronsElem2;
        this->numOuterElectrons += shared;
        other->numOuterElectrons += shared;

        this->shared = shared;
        other->shared = shared;
        this->bondType = COVALENT;
        other->bondType = COVALENT;
		return true;
    }	
	//reverse case of the above.
    else if (strcmp(this->group, "halogen") == 0 &&
	    strcmp(this->symbol, "H") == 0)
    {
        //for any halogen bonding with hydrogen, 2 electrons will be shared between the two.
        int sharedElectronsElem1 = 8 - other->numOuterElectrons;
        other->numOuterElectrons -= sharedElectronsElem1;
        int sharedElectronsElem2 = 2 - this->numOuterElectrons;
        this->numOuterElectrons -= sharedElectronsElem2;

        int shared = sharedElectronsElem1 + sharedElectronsElem2;
        this->numOuterElectrons += shared;
        other->numOuterElectrons += shared;

        this->shared = shared;
        other->shared = shared;
        this->bondType = COVALENT;
        this->bondType = COVALENT;
	    return true;
    }
		
	//The difference in electronegativity is 1.68, which is very near to 1.7
	//This is an Ionic bond but a special case in which the difference isn't >= 1.7
	//which is why we have a hard coded case (possible to clean up in the future?)
	else if (strcmp(this->symbol, "Li") == 0 &&
		strcmp(other->symbol, "I") == 0)
    {
        int electronsDonated = 8 - this->numOuterElectrons;
        other->numOuterElectrons += electronsDonated;
        this->numOuterElectrons -+ electronsDonated;
        this->bondType = IONIC;
        other->bondType = IONIC;
		return true;
    }
	
    //reverse case of the above 	
    else if (strcmp(this->symbol, "I") == 0 &&
        strcmp(other->symbol, "Li") == 0)
    {   
        int electronsDonated = 8 - other->numOuterElectrons;
        this->numOuterElectrons += electronsDonated;
        other->numOuterElectrons -= electroNegativity;
        this->bondType = IONIC;
        other->bondType = IONIC;
        return true;
    }

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
        this->bondType = IONIC;
        this->bondType = IONIC;
		return true;
	}
		
	//covalent
	else if (maxNegativity - minNegativity <= 1.7)
    {
        this->bondType = COVALENT;
        this->bondType = COVALENT;
		return true;
    }
	return false;
}

/* Checks reactions that require 3 elements to form a bond) 

This algorithm was written with help by:
Christopher Culbertson, Associate Professor at Kansas State University
Michael Ayala, Chemistry Major at UC Davis */
bool Element::ReactWith(Element* other1, Element* other2)
{
    //If we have an alkali earth metal, there is a chance we can have a bond.
    //initial check to ensure we don't waste our time.
    if(strcmp(this->group, "alkaliEarth") == 0 ||
        strcmp(other1->group, "alkaliEarth") == 0 ||
        strcmp(other2->group, "alkaliEarth") == 0)
    {
        //If we have two halogens of the same type (plus an alkali earth already checked above, we have a covalent bond)
        if(((strcmp(this->group, "halogen") == 0 && strcmp(other1->group, "halogen") == 0 && strcmp(this->name, other1->name) == 0) ||
            (strcmp(this->group, "halogen") == 0 && strcmp(other2->group, "halogen") == 0 && strcmp(this->name, other2->name) == 0) ||
            (strcmp(other1->group, "halogen") == 0 && strcmp(other2->group, "halogen") == 0 && strcmp(other1->name, other2->name) == 0)))
        {
            if(strcmp(this->group, "alkaliEarth") == 0)
            {
                this->shared = 4;
                other1->shared = 2;
                other2->shared = 2;
            }
            else if (strcmp(other1->group, "alkaliEarth") == 0)
            {
                this->shared = 2;
                other1->shared = 4;
                other2->shared = 2;
            }
            else 
            {
                this->shared = 2;
                other1->shared = 2;
                other2->shared = 4;
            }
            this->bondType = COVALENT;
            other1->bondType = COVALENT;
            other2->bondType = COVALENT;
            return true;
        }

        //If we have two hydrogens and either Beryllium or Magnesium, we have a covalent bond.  If statement has a lot of overhead, look for more efficient ways in the future
        else if (((strcmp(this->symbol, "Be") == 0 || strcmp(this->symbol, "Mg") == 0) && strcmp(other1->symbol, "H") == 0 && strcmp(other2->symbol, "H") == 0) ||
                    (strcmp(this->symbol, "H") == 0 && (strcmp(other1->symbol, "Be") == 0 || strcmp(other1->symbol, "Mg") == 0) && strcmp(other2->symbol, "H") == 0) ||
                    (strcmp(this->symbol, "H") == 0 && strcmp(other1->symbol, "H") == 0 && (strcmp(other2->symbol, "B") == 0 || strcmp(other2->symbol, "Mg") == 0)))
        {
            if(strcmp(this->symbol, "Be") == 0 || strcmp(this->symbol, "Mg") == 0)
            {
                this->shared = 4;
                other1->shared = 2;
                other2->shared = 2;
            }
            else if (strcmp(this->symbol, "Be") == 0 || strcmp(this->symbol, "Mg") == 0)
            {
                this->shared = 2;
                other1->shared = 4;
                other2->shared = 2;
            }
            else
            {
                this->shared = 2;
                other1->shared = 2;
                other2->shared = 4;
            }
            this->bondType = COVALENT;
            other1->bondType = COVALENT;
            other2->bondType = COVALENT;
            return true;
        }

        //If we have two hydrogens plus the other alkali earth metals, we have a ionic bond.  Since we've already ensured we have an alkali earth metal, we can simply check for two hydrogens.
        else if ((strcmp(this->symbol, "H") == 0 && strcmp(other1->symbol, "H") == 0) ||
                    (strcmp(this->symbol, "H") == 0 && strcmp(other2->symbol, "H") == 0) ||
                    (strcmp(other1->symbol, "H") == 0 && strcmp(other2->symbol, "H") == 0))
        {
            this->bondType = IONIC;
            other1->bondType = IONIC;
            other2->bondType = IONIC;
            return true;
        }
        
    }
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

    //Alkali Earth Metals
    Element("Beryllium", "Be", "alkaliEarth", 4, 9.0121831, 2, 1.57),
    Element("Magnesium", "Mg", "alkaliEarth", 12, 24.305, 2, 1.31),
	Element("Calcium", "Ca", "alkaliEarth", 20, 40.078, 2, 1.0),
	Element("Strontium", "Sr", "alkaliEarth", 38, 87.60, 2, 0.95),
	Element("Barium", "Ba", "alkaliEarth", 56, 137.327, 2, 0.89),
    
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
    
    //alkali earth metals
    Element("Beryllium", "Be", "alkaliEarth", 4, 9.0121831, 2, 1.27),
    Element("Magnesium", "Mg", "alkaliEarth", 12, 24.305, 2, 1.31),
    Element("Calcium", "Ca", "alkaliEarth", 20, 40.078, 2, 1.0),
    Element("Strontium", "Sr", "alkaliEarth", 38, 87.62, 2, 0.95),
    Element("Barium", "Ba", "alkaliEarth", 56, 137.327, 2, 0.89),
    Element("Radium", "Ra", "alkaliEarth", 88, 226, 2, 0.9), //not sure if needed
    
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
