#include "alkali.h"

using namespace std;



Alkali::Alkali(void)
{
	createAlkali();
}


void Alkali::createAlkali(void)
{
/*
	extern struct Lithium lithium;
	lithium.elementName = "Lithium";
	lithium.elementSymbol = "Li";
	lithium.atomicNumber = 3;
	lithium.elementWeight = 6.94;
	lithium.numOuterElectrons = 1;
	
	
	extern struct Sodium sodium;
	sodium.elementName = "Sodium";
	sodium.elementSymbol = "Na";
	sodium.atomicNumber = 11;
	sodium.elementWeight = 22.98976928;
	sodium.numOuterElectrons = 1;
	
	extern struct Potassium potassium;
	potassium.elementName = "Potassium";
	potassium.elementSymbol = "K";
	potassium.atomicNumber = 19;
	potassium.elementWeight = 39.0938;
	potassium.numOuterElectrons = 1;
	
	
	extern struct Rubidium rubidium;
	rubidium.elementName = "Rubidium";
	rubidium.elementSymbol = "Rb";
	rubidium.atomicNumber = 37;
	rubidium.elementWeight = 85.4678;
	rubidium.numOuterElectrons = 1;
	
	extern struct Cesium cesium;
	cesium.elementName = "Cesium";
	cesium.elementSymbol = "Cs";
	cesium.atomicNumber = 55;
	cesium.elementWeight = 132.90545196;
	cesium.numOuterElectrons = 1;
	
	extern struct Francium francium;
	francium.elementName = "Francium";
	francium.elementSymbol = "Fr";
	francium.atomicNumber = 87;
	francium.elementWeight = 223;
	francium.numOuterElectrons = 1;
	*/
}

void Alkali::checkBond(int outerNum1, int outerNum2)
{
	bool bond = (outerNum1 + outerNum2 == 8) ? true : false;
	return bond;
}