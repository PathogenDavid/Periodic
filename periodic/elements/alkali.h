#include "element.h"

#ifndef ALKALI_H
#define ALKALI_H

struct Lithium
{
	std::string elementName;
	short atomicNumber;
	double elementWeight;
	short numOuterElectrons;
};

struct Sodium
{
	std::string elementName;
	short atomicNumber;
	double elementWeight;
	short numOuterElectrons;
};

struct Potassium
{
	std::string elementName;
	short atomicNumber;
	double elementWeight;
	short numOuterElectrons;
};

struct Rubidium
{
	std::string elementName;
	short atomicNumber;
	double elementWeight;
	short numOuterElectrons;
};

struct Cesium
{
	std::string elementName;
	short atomicNumber;
	double elementWeight;
	short numOuterElectrons;
};

struct Francium
{
	std::string elementName;
	short atomicNumber;
	double elementWeight;
	short numOuterElectrons;
};

class Alkali : public Element
{
	public:
		Alkali(void)
		virtual void print(void);
}



#endif