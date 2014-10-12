#include "element.h"

#ifndef HALOGEN_H
#define HALOGEN_H

struct Flourine
{
	std::string elementName, elementSymbol;
	short atomicNumber;
	double elementWeight;
	short numOuterElectrons;
};

struct Chlorine
{
	std::string elementName, elementSymbol;
	short atomicNumber;
	double elementWeight;
	short numOuterElectrons;
};

struct Bromine
{
	std::string elementName, elementSymbol;
	short atomicNumber;
	double elementWeight;
	short numOuterElectrons;
};

struct Iodine
{
	std::string elementName, elementSymbol;
	short atomicNumber;
	double elementWeight;
	short numOuterElectrons;
};

struct Astatine
{
	std::string elementName, elementSymbol;
	short atomicNumber;
	double elementWeight;
	short numOuterElectrons;
};

class Halogen : public Element
{
	public:
		Halogen(void);
		void createHalogen(void);
		bool checkBond(int, int); 
		virtual void print(void);
}	


#endif