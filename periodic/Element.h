#ifndef ELEMENT_H
#define ELEMENT_H

/*Enumeration to keep track of the state of element at the current bonding process.  Potential is in there to show if a bond may have the potential to bond. */
enum bondState { NONE, IONIC, COVALENT, POTENTIAL };

class Element
{
    private:
        Element* baseElement;
        const char* name;
        const char* symbol;
		const char* group;
        short atomicNumber;
        double elementWeight;
        int numOuterElectrons;
		double electroNegativity;
        bondState bondType;
        int sharedElectrons;
    public:
        Element();
        Element(const char* name, const char* symbol, const char* group, short atomicNumber, double elementWeight, int numOuterElectrons, double electroNegativity);
        Element(Element* baseElement);

        const char* GetName();
        const char* GetSymbol();
		const char* GetGroup();
        short GetAtomicNumber();
        double GetElementWeight();
        int GetNumOuterElectrons();
		double GetElectroNegativity();
        bondState GetBondType();
        int GetSharedElectrons();

        bool IsRawElement();

        int GetCharge();

        static void GetRawElement(int num, Element* elementOut);
        static bool GetRawElement(const char* name, Element* elementOut);
        static int GetRawElementNum(const char* name);
        static int GetRawElementCount();

        void ResetToBasicState();

        bool ReactWith(Element* other);
		bool ReactWith(Element* other1, Element* other2);
};

#endif
