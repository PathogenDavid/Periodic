#ifndef ELEMENT_H
#define ELEMENT_H

/*Enumeration to keep track of the state of element at the current bonding process.  Potential is in there to show if a bond may have the potential to bond. */
enum bondState { NONE, IONIC, COVALENT, POTENTIAL };

/*Enumeration to keep track of the group a element belongs in */
enum groupState {ALKALI, ALKALIEARTH, HALOGEN, NOBLE, HYDROGEN, NONMETAL };

//! Element represents a chemical element
class Element
{
    private:
        //! NULL if this element is in its natural state, otherwise a pointer to the Element instance that this Element is derived from.
        //! Note: Even if this is a derived element, it may still be in its natural state.
        Element* baseElement;
        //! The human-readable name for this element
        const char* name;
        //! The symbol from the periodic table for this element
        const char* symbol;
        //! What group this element is from
		groupState group;
        //! The atomic number for this element
        short atomicNumber;
        //! The atomic weight of this element
        double elementWeight;
        //! The number of outer electrons for this element
        int numOuterElectrons;
        //! The electronegativity for this element
		double electroNegativity;
        //! The type of bond that this element participates in (if any.)
        bondState bondType;
        //! The number of electrons this element is sharing with neighboring elements.
        int sharedElectrons;
    public:
        //! Creates a dead element, elements made with this constructor must be initialized with one of the static pseudoconstructors before use.
        Element();
        //! Creates an element with the specified initial attributes, the element is assumed to be in its default state.
        Element(const char* name, const char* symbol, groupState group, short atomicNumber, double elementWeight, int numOuterElectrons, double electroNegativity);
        //! Creates an element from the specified element
        Element(Element* baseElement);

        //! Returns the human-readable name of this element
        const char* GetName();
        //! Returns the symbol from the periodic table for this element
        const char* GetSymbol();
        //! Returns the group this element belongs to
		groupState GetGroup();
        //! Retuns the atomic number for this element
        short GetAtomicNumber();
        //! Returns the atomic weight of this element
        double GetElementWeight();
        //! Returns the number of outer electrons this element has
        int GetNumOuterElectrons();
        //! Returns the electronegativity of this element
		double GetElectroNegativity();
        //! Returns the type of bond this element pariticpates in, if any
        bondState GetBondType();
        //! Gets the number electrons this element shares with its neighbors
        int GetSharedElectrons();

        //! Returns true if this element is in its raw, natural state
        bool IsRawElement();

        //! Returns the charge on this element
        int GetCharge();

        //! Pseudoconstructor for initializing the specified Element with the Element at the given index
        static void GetRawElement(int num, Element* elementOut);
        //! Pseudoconstructor for initializing the specified Element with the Element with the given symbol on the periodic table
        static bool GetRawElement(const char* name, Element* elementOut);
        //! Utility function for getting the index of the natural Element with the  given symbol on the periodic table
        static int GetRawElementNum(const char* name);
        //! Returns the number of raw elements in their natural state that this program knows about
        static int GetRawElementCount();

        //! Resets this element to its natural state
        void ResetToBasicState();

        //! Reacts this element with another one
        bool ReactWith(Element* other);
        //! Reacts thie element with two other elements
		bool ReactWith(Element* other1, Element* other2);
};

#endif
