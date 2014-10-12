#include "halogen.h"

using namespace std;



Halogen::Halogen(void)
{
	numOuterElectrons = 7;
}


void Halogen::createHalogen(void);
{
	struct Flourine flourine;
	flourine.elementName = "Flourine";
	flourine.elementSymbol = "F";
	flourine.atomicNumber = 9;
	flourine.elementWeight = 18.998403163;
	flourine.numOuterElectrons = 7;
	
	struct Chlorine chorline;
	chroline.elementName = "Chroline";
	chroline.elementSymbol = "Cl";
	chroline.atomicNumber = 17;
	chroline.elementWeight = 35.45;
	chroline.numOuterElectrons = 7;
	
	struct Bromine bromine;
	bromine.elementName = "Bromine";
	bromine.elementSymbol = "Br";
	bromine.atomicNumber = 35;
	bromine.elementWeight = 79.094;
	bromine.numOuterElectrons = 7;
	
	struct Iodine iodine;
	iodine.elementName = "Iodine";
	iodine.elementSymbol = "I";
	iodine.atomicNumber = 53;
	iodine.elementWeight = 126.90447;
	iodine.numOuterElectrons = 7;
	
	struct Astatine astatine;
	astatine.elementName = "Astatine";
	astatine.elementSymbol = "At";
	astatine.atomicNumber = 85;
	astatine.elementWeight = 210;
	astatine.numOuterElectrons = 7;
}