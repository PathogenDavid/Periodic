#include "element.h"



Element::Element(void)
{
	
}

void Element::createElements(void)
{
	//alkali
	static struct Element lithium;
	lithium.elementName = "Lithium";
	lithium.elementSymbol = "Li";
	lithium.atomicNumber = 3;
	lithium.elementWeight = 6.94;
	lithium.numOuterElectrons = 1;
	
	static struct Element sodium;
	lithium.elementName = "Lithium";
	lithium.elementSymbol = "Li";
	lithium.atomicNumber = 3;
	lithium.elementWeight = 6.94;
	lithium.numOuterElectrons = 1;
	
	static struct Element potassium;
	potassium.elementName = "Potassium";
	potassium.elementSymbol = "K";
	potassium.atomicNumber = 19;
	potassium.elementWeight = 39.0938;
	potassium.numOuterElectrons = 1;
	
	static struct Element rubidium;
	rubidium.elementName = "Rubidium";
	rubidium.elementSymbol = "Rb";
	rubidium.atomicNumber = 37;
	rubidium.elementWeight = 85.4678;
	rubidium.numOuterElectrons = 1;
	
	//don't remember if we need this one or not
	static struct Element cesium;
	cesium.elementName = "Cesium";
	cesium.elementSymbol = "Cs";
	cesium.atomicNumber = 55;
	cesium.elementWeight = 132.90545196;
	cesium.numOuterElectrons = 1;
	
	
	
	//halogen
	static struct Element flourine;
	flourine.elementName = "Flourine";
	flourine.elementSymbol = "F";
	flourine.atomicNumber = 9;
	flourine.elementWeight = 18.998403163;
	flourine.numOuterElectrons = 7;
	
	static struct Element chlorine;
	chroline.elementName = "Chlorine";
	chroline.elementSymbol = "Cl";
	chroline.atomicNumber = 17;
	chroline.elementWeight = 35.45;
	chroline.numOuterElectrons = 7;
	
	static struct Element bormine;
	bromine.elementName = "Bromine";
	bromine.elementSymbol = "Br";
	bromine.atomicNumber = 35;
	bromine.elementWeight = 79.094;
	bromine.numOuterElectrons = 7;
	
	static struct Element iodine;
	iodine.elementName = "Iodine";
	iodine.elementSymbol = "I";
	iodine.atomicNumber = 53;
	iodine.elementWeight = 126.90447;
	iodine.numOuterElectrons = 7;
	
	//noble gases
	static struct Element helium;
	iodine.elementName = "Helium";
	iodine.elementSymbol = "He";
	iodine.atomicNumber = 2;
	iodine.elementWeight = 4.002602;
	iodine.numOuterElectrons = 2;
	
	static struct Element neon;
	iodine.elementName = "Neon";
	iodine.elementSymbol = "Ne";
	iodine.atomicNumber = 10;
	iodine.elementWeight = 20.1797;
	iodine.numOuterElectrons = 8;
	
	static struct Element argon;
	iodine.elementName = "Argon";
	iodine.elementSymbol = "Ar";
	iodine.atomicNumber = 18;
	iodine.elementWeight = 39.948;
	iodine.numOuterElectrons = 8;
	
	static struct Element krypton;
	iodine.elementName = "Krypton";
	iodine.elementSymbol = "Kr";
	iodine.atomicNumber = 36;
	iodine.elementWeight = 83.798;
	iodine.numOuterElectrons = 8;
}