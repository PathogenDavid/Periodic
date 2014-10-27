#ifndef ELEMENT_H
#define ELEMENT_H

class Element
{
    private:
        Element* baseElement;
        const char* name;
        const char* symbol;
        short atomicNumber;
        double elementWeight;
        int numOuterElectrons;
    public:
        Element();
        Element(const char* name, const char* symbol, short atomicNumber, double elementWeight, int numOuterElectrons);
        Element(Element* baseElement);

        const char* GetName();
        const char* GetSymbol();
        short GetAtomicNumber();
        double GetElementWeight();
        int GetNumOuterElectrons();

        bool IsRawElement();

        int GetCharge();

        static void GetRawElement(int num, Element* elementOut);
        static int GetRawElementCount();

        void ResetToBasicState();

        bool ReactWith(Element* other);
};

#endif
