//#include<string>

#ifndef ELEMENT_H
#define ELEMENT_H

struct Flourine
{

}

struct Chlorine
{

}

struct Bromine
{

}

struct Iodine
{

}

struct Astatine
{

}

class Element
{
	public:
		virtual short atomicNumber;
		virtual string elementSymbol;
		virtual string elementName;
		virtual double elementWeight;
		virtual int numOuterElectrons;
}


#endif
