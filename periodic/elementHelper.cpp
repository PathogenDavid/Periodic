#include elementHelper.h

bool elementHelper::doesElementBond(Element* elem1, Element* elem2)
{
	if(elem1.numOuterElectrons + elem2.numOuterElectrons == 8)
		return true;
	else return false;
	
}