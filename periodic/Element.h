#ifndef ELEMENT_H
#define ELEMENT_H

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

        bool IsRawElement();

        int GetCharge();

        static void GetRawElement(int num, Element* elementOut);
        static bool GetRawElement(const char* name, Element* elementOut);
        static int GetRawElementNum(const char* name);
        static int GetRawElementCount();

        void ResetToBasicState();

        bool ReactWith(Element* other);
};

#endif
