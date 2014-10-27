#include "periodic.h"
#include "Element.h"
#include <sifteo.h>

// Default constructor will not create a valid element, it must be initialized before use using GetRawElement
Element::Element()
{
}

Element::Element(const char* name, const char* symbol, short atomicNumber, double elementWeight, int numOuterElectrons)
{
    this->baseElement = NULL;
    this->name = name;
    this->symbol = symbol;
    this->atomicNumber = atomicNumber;
    this->elementWeight = elementWeight;
    this->numOuterElectrons = numOuterElectrons;
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
}

const char* Element::GetName() { return name; }
const char* Element::GetSymbol() { return symbol; }
short Element::GetAtomicNumber() { return atomicNumber; }
double Element::GetElementWeight() { return elementWeight; }
int Element::GetNumOuterElectrons() { return numOuterElectrons; }

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

    //Check for ionic bond:
    if ((this->numOuterElectrons + other->numOuterElectrons) == 8)
    {
        //Check if we should be an electron donor. (This is probably not scientficially accurate.)
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

    return false;
}

static Element rawElements[] =
{
    //Alkali Metals
    Element("Lithium", "Li", 3, 6.94, 1),
    Element("Sodium", "Na", 11, 22.9898, 1),
    Element("Potassium", "K", 19, 39.0938, 1),
    Element("Rubidium", "Rb", 37, 85.4678, 1),
    Element("Cesium", "Cs", 55, 132.90545196, 1), //don't remember if we need this one or not
    
    //Halogens
    Element("Flourine", "F", 9, 18.998403163, 7),
    Element("Chlorine", "Cl", 17, 35.45, 7),
    Element("Bromine", "Br", 35, 79.094, 7),
    Element("Iodine", "I", 53, 126.90447, 7),
    
    //Noble Gases
    Element("Helium", "He", 2, 4.002602, 2),
    Element("Neon", "Ne", 10, 20.1797, 8),
    Element("Argon", "Ar", 18, 39.948, 8),
    Element("Krypton", "Kr", 36, 83.798, 8),
};

void Element::GetRawElement(int num, Element* elementOut)
{
    Assert(num >= 0 && num < GetRawElementCount());
    *elementOut = Element(&rawElements[num]);
}

int Element::GetRawElementCount()
{
    return CountOfArray(rawElements);
}
