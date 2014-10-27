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
        Element(const char* name, const char* symbol, short atomicNumber, double elementWeight, int numOuterElectrons);
        Element(Element* baseElement);

        const char* GetName();
        const char* GetSymbol();
        short GetAtomicNumber();
        double GetElementWeight();
        int GetNumOuterElectrons();

        bool IsRawElement();

        int GetCharge();

        static Element* GetRawElement(int num);
        static int GetRawElementCount();

        void ResetToBasicState();

        void ReactWith(Element* other);
};

#endif
