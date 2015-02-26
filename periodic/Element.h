#ifndef __ELEMENT_H__
#define __ELEMENT_H__

#include "Bond.h"
#include "BondSolution.h"

class Reaction;
class Compound;

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
        //! The number of electrons this element is sharing with neighboring elements.
        int sharedElectrons;

        Bond bonds[BondSide_Count];
        Reaction* currentReaction;
        Compound* currentCompound;
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
        //bool ReactWith(Element* other);
        //! Reacts this element with two other elements
		//bool ReactWith(Element* other1, Element* other2);

        void AddBond(BondSide side, Element* with);
        void SetReaction(Reaction* reaction);
        
        BondType GetBondTypeFor(Compound* compound, BondSide side);
        int GetBondDataFor(Compound* compound, BondSide side);
        void SetBondTypeFor(Compound* compound, BondSide side, BondType type, int data, int otherData);
        void SetBondTypeFor(Compound* compound, BondSide side, BondType type, int data);
        void SetBondTypeFor(Compound* compound, BondSide side, BondType type);
        void SetBondTypeFor(Compound* compound, Element* otherElement, BondType type, int data, int otherData);
        void SetBondTypeFor(Compound* compound, Element* otherElement, BondType type, int data);
        void SetBondTypeFor(Compound* compound, Element* otherElement, BondType type);

        BondType GetBondTypeFor(BondSide side);
        int GetBondDataFor(BondSide side);

        bool HasBondType(BondType type);

        Element* GetBondWith(BondSide side);
        Element* GetBondWith(groupState group);
        Element* GetBondWith(const char* symbol);

        bool GetBondWith(BondSide side, Element** element_out);
        bool GetBondWith(groupState group, Element** element_out);
        bool GetBondWith(const char* symbol, Element** element_out);

        bool HasBondWith(groupState group);
        bool HasBondWith(const char* symbol);

        BondSide SideOf(Element* otherElement);

        void ApplyCompound(Compound* compound);
};

#endif
