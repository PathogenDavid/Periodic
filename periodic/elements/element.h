#ifndef ELEMENT_H
#define ELEMENT_H

struct Element
{
    string elementName;
    string elementSymbol;
    short atomicNumber;
    double elementWeight;
    short numOuterElectrons;
}

class Element
{
    private:
        const char* elementName;
        const char* elementSymbol;
        short atomicNumber;
        double elementWeight;
        int numOuterElectrons;
    public:
        Element(void);
        createElements(void);
}

#endif
